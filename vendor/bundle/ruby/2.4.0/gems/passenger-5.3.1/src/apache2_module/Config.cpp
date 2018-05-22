/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2010-2017 Phusion Holding B.V.
 *
 *  "Passenger", "Phusion Passenger" and "Union Station" are registered
 *  trademarks of Phusion Holding B.V.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#include <algorithm>
#include <cstdlib>
#include <climits>

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

/* ap_config.h checks whether the compiler has support for C99's designated
 * initializers, and defines AP_HAVE_DESIGNATED_INITIALIZER if it does. However,
 * g++ does not support designated initializers, even when ap_config.h thinks
 * it does. Here we undefine the macro to force httpd_config.h to not use
 * designated initializers. This should fix compilation problems on some systems.
 */
#include <ap_config.h>
#undef AP_HAVE_DESIGNATED_INITIALIZER

#include "Config.h"
#include "ConfigGeneral/SetterFuncs.h"
#include "ConfigGeneral/ManifestGeneration.h"
#include "DirConfig/AutoGeneratedCreateFunction.cpp"
#include "DirConfig/AutoGeneratedMergeFunction.cpp"
#include "Utils.h"
#include <JsonTools/Autocast.h>
#include <Utils.h>
#include <Constants.h>

// The APR headers must come after the Passenger headers.
// See Hooks.cpp to learn why.
#include <apr_strings.h>
// In Apache < 2.4, this macro was necessary for core_dir_config and other structs
#define CORE_PRIVATE
#include <http_core.h>
#include <http_config.h>
#include <http_log.h>


extern "C" module AP_MODULE_DECLARE_DATA passenger_module;

#ifdef APLOG_USE_MODULE
	APLOG_USE_MODULE(passenger);
#endif

#ifndef ap_get_core_module_config
	#define ap_get_core_module_config(s) ap_get_module_config(s, &core_module)
#endif


namespace Passenger {
namespace Apache2Module {

using namespace std;


typedef const char * (*Take1Func)();
typedef const char * (*Take2Func)();
typedef const char * (*FlagFunc)();

ServerConfig serverConfig;


template<typename T> static apr_status_t
destroyConfigStruct(void *x) {
	delete (T *) x;
	return APR_SUCCESS;
}

template<typename Collection, typename T> static bool
contains(const Collection &coll, const T &item) {
	typename Collection::const_iterator it;
	for (it = coll.begin(); it != coll.end(); it++) {
		if (*it == item) {
			return true;
		}
	}
	return false;
}

static DirConfig *
createDirConfigStruct(apr_pool_t *pool) {
	DirConfig *config = new DirConfig();
	apr_pool_cleanup_register(pool, config, destroyConfigStruct<DirConfig>, apr_pool_cleanup_null);
	return config;
}

void *
createDirConfig(apr_pool_t *p, char *dirspec) {
	DirConfig *config = createDirConfigStruct(p);

	createDirConfig_autoGenerated(config);

	return config;
}

void *
mergeDirConfig(apr_pool_t *p, void *basev, void *addv) {
	DirConfig *config = createDirConfigStruct(p);
	DirConfig *base = (DirConfig *) basev;
	DirConfig *add = (DirConfig *) addv;

	mergeDirConfig_autoGenerated(config, base, add);

	return config;
}

void
postprocessConfig(server_rec *s, apr_pool_t *pool, apr_pool_t *temp_pool) {
	if (serverConfig.defaultGroup.empty()) {
		struct passwd *userEntry = getpwnam(serverConfig.defaultUser.c_str());
		if (userEntry == NULL) {
			throw ConfigurationException(
				"The user that PassengerDefaultUser refers to, '"
				+ serverConfig.defaultUser
				+ "', does not exist.");
		}

		struct group *groupEntry = getgrgid(userEntry->pw_gid);
		if (groupEntry == NULL) {
			throw ConfigurationException(
				"The option PassengerDefaultUser is set to '"
				+ serverConfig.defaultUser
				+ "', but its primary group doesn't exist. "
				"In other words, your system's user account database "
				"is broken. Please fix it.");
		}

		serverConfig.defaultGroup = apr_pstrdup(pool, groupEntry->gr_name);
	}

	serverConfig.manifest = ConfigManifestGenerator(s, temp_pool).execute();

	if (!serverConfig.dumpConfigManifest.empty()) {
		FILE *f = fopen(serverConfig.dumpConfigManifest.c_str(), "w");
		if (f == NULL) {
			fprintf(stderr, "Error opening %s for writing\n",
				serverConfig.dumpConfigManifest.c_str());
		} else {
			string dumpContent = serverConfig.manifest.toStyledString();
			size_t ret = fwrite(dumpContent.data(), 1, dumpContent.size(), f);
			(void) ret; // Ignore compilation warning.
			fclose(f);
		}
	}
}


/*************************************************
 * Passenger settings
 *************************************************/

#include "ConfigGeneral/AutoGeneratedSetterFuncs.cpp"

static const char *
cmd_passenger_enterprise_only(cmd_parms *cmd, void *pcfg, const char *arg) {
	return "this feature is only available in Phusion Passenger Enterprise. "
		"You are currently running the open source Phusion Passenger Enterprise. "
		"Please learn more about and/or buy Phusion Passenger Enterprise at https://www.phusionpassenger.com/enterprise";
}

static const char *
cmd_passenger_ctl(cmd_parms *cmd, void *dummy, const char *name, const char *value) {
	const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
	if (err != NULL) {
		ap_log_perror(APLOG_MARK, APLOG_STARTUP, 0, cmd->temp_pool,
			"WARNING: %s", err);
	}

	serverConfig.ctlSourceFile = cmd->directive->filename;
	serverConfig.ctlSourceLine = cmd->directive->line_num;
	serverConfig.ctlExplicitlySet = true;
	try {
		serverConfig.ctl[name] = autocastValueToJson(value);
		return NULL;
	} catch (const Json::Reader &) {
		return "Error parsing value as JSON";
	}
}

static const char *
cmd_passenger_spawn_method(cmd_parms *cmd, void *pcfg, const char *arg) {
	const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES);
	if (err != NULL) {
		return err;
	}

	DirConfig *config = (DirConfig *) pcfg;
	config->mSpawnMethodSourceFile = cmd->directive->filename;
	config->mSpawnMethodSourceLine = cmd->directive->line_num;
	config->mSpawnMethodExplicitlySet = true;
	if (strcmp(arg, "smart") == 0 || strcmp(arg, "smart-lv2") == 0) {
		config->mSpawnMethod = "smart";
	} else if (strcmp(arg, "conservative") == 0 || strcmp(arg, "direct") == 0) {
		config->mSpawnMethod = "direct";
	} else {
		return "PassengerSpawnMethod may only be 'smart', 'direct'.";
	}
	return NULL;
}

static const char *
cmd_passenger_base_uri(cmd_parms *cmd, void *pcfg, const char *arg) {
	const char *err = ap_check_cmd_context(cmd, NOT_IN_FILES);
	if (err != NULL) {
		return err;
	}

	DirConfig *config = (DirConfig *) pcfg;
	config->mBaseURIsSourceFile = cmd->directive->filename;
	config->mBaseURIsSourceLine = cmd->directive->line_num;
	config->mBaseURIsExplicitlySet = true;
	if (strlen(arg) == 0) {
		return "PassengerBaseURI may not be set to the empty string";
	} else if (arg[0] != '/') {
		return "PassengerBaseURI must start with a slash (/)";
	} else if (strlen(arg) > 1 && arg[strlen(arg) - 1] == '/') {
		return "PassengerBaseURI must not end with a slash (/)";
	} else {
		config->mBaseURIs.insert(arg);
		return NULL;
	}
}

extern "C" const command_rec passenger_commands[] = {
	#include "ConfigGeneral/AutoGeneratedDefinitions.cpp"
	{ NULL }
};


} // namespace Apache2Module
} // namespace Passenger
