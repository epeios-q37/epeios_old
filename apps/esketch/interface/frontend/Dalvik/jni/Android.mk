# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifndef EPEIOS_SRC
	hdr += ../epeios
else
	hdr += ../../../../../stable/
endif

src := ../$(hdr)

EPEIOS_MODULES = cdgb64 cio err fil flw flx fnm lcl mmi rgstry str tol ttr uym xml xpp
EPEIOS_MODULES += dalvik dvkbse dvkfev
EPEIOS_MODULES += jvabse
EPEIOS_MODULES += scldalvik scllocale sclmisc sclrgstry sclerror

MODULES += ui ui_base ui_main ui_ssn_frm
MODULES += trunk


EPEIOS_SOURCE_FILES = $(EPEIOS_MODULES:%=$(src)%.cpp)
SOURCE_FILES = $(MODULES:%=%.cpp)

LOCAL_CPPFLAGS := -DCPE_JAVA -DERR_JMPUSE

LOCAL_C_INCLUDES := $(hdr) .. ../.. ../../..
LOCAL_MODULE    := esketchdvk
LOCAL_SRC_FILES := $(LOCAL_MODULE).cpp $(SOURCE_FILES) $(EPEIOS_SOURCE_FILES)

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
