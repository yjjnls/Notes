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

#include "pipejoint.h"


PipeJoint make_pipe_joint(const std::string &media_type, const std::string &name)
{
    std::string name_ = name;
    if (name_.empty()) {
        static int id = 0;
        name_ = std::to_string(id++);
    }

    GstElement *psink = gst_element_factory_make("proxysink", (name_ + "_proxysink").c_str());
    GstElement *psrc = gst_element_factory_make("proxysrc", (name_ + "_proxysrc").c_str());
    g_object_set(psrc, "proxysink", psink, NULL);
    g_object_set_data(G_OBJECT(psink), "media-type", (gchar *)media_type.c_str());
    g_object_set_data(G_OBJECT(psrc), "media-type", (gchar *)media_type.c_str());

    ///-----------------
    PipeJoint pipejoint;
    pipejoint.upstream_joint = psink;
    pipejoint.downstream_joint = psrc;
    return pipejoint;
}
void update_downstream_joint(PipeJoint *pipejoint)
{
    GstElement *psrc = gst_element_factory_make("proxysrc", NULL);
    g_object_set(psrc, "proxysink", pipejoint->upstream_joint, NULL);
    pipejoint->downstream_joint = psrc;
}

void destroy_pipe_joint(PipeJoint *pipejoint)
{
    pipejoint->upstream_joint = NULL;
    pipejoint->downstream_joint = NULL;
}
