require 'mkmf'

dir_config 'mruby'

have_library 'mruby' or abort 'missing mruby library'

def require_header header
  have_header header or abort "missing #{header}"
end

require_header 'mruby.h'

create_header
create_makefile 'mruby/mruby'

