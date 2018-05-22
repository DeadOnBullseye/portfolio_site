# -*- encoding: utf-8 -*-
# stub: lazyload-rails 0.3.1 ruby lib

Gem::Specification.new do |s|
  s.name = "lazyload-rails".freeze
  s.version = "0.3.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["Javier Saldana".freeze]
  s.date = "2014-11-17"
  s.description = "lazyload-rails project integrates jQuery Lazy Load Plugin for Rails image_tag helpers".freeze
  s.email = "javier@tractical.com".freeze
  s.homepage = "https://github.com/jassa/lazyload-rails".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "2.6.14".freeze
  s.summary = "jQuery Lazy Load for Rails image_tag helpers".freeze

  s.installed_by_version = "2.6.14" if s.respond_to? :installed_by_version

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<nokogiri>.freeze, ["~> 1.5"])
      s.add_development_dependency(%q<actionpack>.freeze, [">= 3.1"])
    else
      s.add_dependency(%q<nokogiri>.freeze, ["~> 1.5"])
      s.add_dependency(%q<actionpack>.freeze, [">= 3.1"])
    end
  else
    s.add_dependency(%q<nokogiri>.freeze, ["~> 1.5"])
    s.add_dependency(%q<actionpack>.freeze, [">= 3.1"])
  end
end
