# Be sure to restart your server when you modify this file.

# Version of your assets, change this if you want to expire all your assets.
Rails.application.config.assets.version = '1.0'

# Add additional assets to the asset load path.
# Rails.application.config.assets.paths << Emoji.images_path
# Add Yarn node_modules folder to the asset load path.
Rails.application.config.assets.paths << Rails.root.join('node_modules')


#CSS
Rails.application.config.assets.precompile += %w( index.css )
Rails.application.config.assets.precompile += %w( nav.css )
Rails.application.config.assets.precompile += %w( home.css )
Rails.application.config.assets.precompile += %w( about.css )
Rails.application.config.assets.precompile += %w( skills.css )
Rails.application.config.assets.precompile += %w( projects.css )
Rails.application.config.assets.precompile += %w( contact.css )


# Javascript
Rails.application.config.assets.precompile += %w( jquery.js )
Rails.application.config.assets.precompile += %w( main.js )
Rails.application.config.assets.precompile += %w( skills.js )
Rails.application.config.assets.precompile += %w( projects.js )
Rails.application.config.assets.precompile += %w( submission.js )


# Precompile additional assets.
# application.js, application.css, and all non-JS/CSS in the app/assets
# folder are already added.
# Rails.application.config.assets.precompile += %w( admin.js admin.css )
