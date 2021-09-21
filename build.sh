#!/bin/bash

cmake \
	-DFFMPEG=$(which ffmpeg)\
	-DBASH=$(which bash)\
	-DPYTHON_INCLUDE_DIR=$(python -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())")  \
        -DPYTHON_LIBRARY=$(python -c "import distutils.sysconfig as sysconfig; print(sysconfig.get_config_var('LIBDIR'))") \
	-DPYBIND11_PYTHON_VERSION=3.6\
	-B./build \
	-S.
