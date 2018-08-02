/*
 * Copyright 2018 KEDACOM Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIBWEBSTREAMER_UTILS_PIPEJOINT_H_
#define _LIBWEBSTREAMER_UTILS_PIPEJOINT_H_

#include <gst/gst.h>
#include <string>

struct PipeJoint
{
    GstElement *upstream_joint = NULL;
    GstElement *downstream_joint = NULL;
};

PipeJoint make_pipe_joint(const std::string &media_type = "video",
                          const std::string &name = "");
void update_downstream_joint(PipeJoint *pipejoint);
void destroy_pipe_joint(PipeJoint *pipejoint);

#endif
