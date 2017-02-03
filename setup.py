from distutils.core import setup

setup(name='itlog',
	  description="""A portable logger for monitoring iterative numerical algorithms""",
	  version='0.1',
	  url='https://github.com/apotschka/itlog',
	  packages=['itlog'],
	  package_dir={'': 'bindings/python'},
)
