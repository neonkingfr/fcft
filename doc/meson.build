sh = find_program('sh', native: true)

scdoc = dependency('scdoc', native: true)
scdoc_prog = find_program(scdoc.get_pkgconfig_variable('scdoc'), native: true)

foreach man_src : ['fcft_clone.3.scd',
                   'fcft_destroy.3.scd',
                   'fcft_from_name.3.scd',
                   'fcft_glyph_rasterize.3.scd',
                   'fcft_kerning.3.scd',
                   'fcft_size_adjust.3.scd']
  parts = man_src.split('.')
  name = parts[-3]
  section = parts[-2]
  out = '@0@.@1@'.format(name, section)

  custom_target(
    out,
    output: out,
    input: man_src,
    command: [sh, '-c', '@0@ < @INPUT@'.format(scdoc_prog.path())],
    capture: true,
    install: true,
    install_dir: join_paths(get_option('mandir'), 'man@0@'.format(section)))
endforeach