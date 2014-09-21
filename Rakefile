# -*- ruby -*-

require 'rubygems'
require 'hoe'
begin
  require 'rake/extensiontask'
rescue LoadError => e
  warn "\nmissing #{e.path} (for rake-compiler)" if e.respond_to? :path
  warn "run: rake newb\n\n"
end

Hoe.plugin :minitest
Hoe.plugin :git

HOE = Hoe.spec 'mruby' do
  developer 'Eric Hodel', 'drbrain@segment7.net'

  rdoc_locations << 'docs.seattlerb.org:/data/www/docs.seattlerb.org/mruby/'

  license 'MIT'

  self.readme_file = 'README.rdoc'
  self.spec_extras[:extensions] = 'ext/mruby/extconf.rb'
end

if Rake.const_defined? :ExtensionTask then
  HOE.spec.files.delete_if { |file| file == '.gemtest' }

  Rake::ExtensionTask.new 'mruby', HOE.spec do |ext|
    ext.lib_dir = 'lib/mruby'
  end

  task test: :compile
end

# vim: syntax=ruby
