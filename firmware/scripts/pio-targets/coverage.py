Import("env")

import os

def coverage_check_gcovr(*args, **kwargs):
  # Ensure the coverage report dependency is available
  try:
    import gcovr
  except:
    env.Execute(
      env.VerboseAction(
        '$PYTHONEXE -m pip install "gcovr"',
        "Installing Python dependencies (gcovr) for coverage tasks",
      )
    )

def coverage_generate_report(*args, **kwargs):
  build_dir = env.subst(env['BUILD_DIR'])
  relative_build_dir = os.path.relpath(build_dir, '.').replace('\\', '/')
  exclude_dir = '/'.join([relative_build_dir, 'test'])
  output_file = '/'.join([relative_build_dir, env.subst('coverage.${PROGNAME}.html')])

  env.Execute("pio test --environment $PIOENV")
  env.Execute(' '.join([
    '$PYTHONEXE -m gcovr --txt --html-self-contained',
    f'--exclude-directories "{exclude_dir}"',
    f'--html-details "{output_file}"',
  ]))
  print(f'Generated coverage report at "{os.path.abspath(output_file)}"')

env.AddCustomTarget(
  "coverage",
  title="Coverage",
  dependencies=None,
  actions=[
    coverage_check_gcovr,
    coverage_generate_report,
  ]
)