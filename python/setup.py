# -*- coding: utf-8 -*-

from setuptools import setup, find_packages
from os import path

here = path.abspath(path.dirname(__file__))

long_description = 'Please see [ayaka14732/librime-python](https://github.com/ayaka14732/librime-python).'

setup(
    name='rimeext',
    version='0.0.1',
    description='Rime Python extension',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/ayaka14732/librime-python',
    author='ayaka14732',
    author_email='ayaka@mail.shn.hk',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Text Processing :: Linguistic',
        'Natural Language :: Chinese (Simplified)',
        'Natural Language :: Chinese (Traditional)',
        'Natural Language :: Cantonese',
        'License :: CC0 1.0 Universal (CC0 1.0) Public Domain Dedication',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
    ],
    keywords='linguistics input-method',
    packages=find_packages('src'),
    package_dir={'': 'src'},
    python_requires='>=3.7, <4',
    entry_points={},
    project_urls={
        'Bug Reports': 'https://github.com/ayaka14732/librime-python/issues',
        'Source': 'https://github.com/ayaka14732/librime-python',
    }
)
