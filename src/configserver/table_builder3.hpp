/*
 * (C) 2007-2010 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id: table_builder3.hpp 155 2011-07-20 06:18:49Z brianzf@126.com $
 *
 * Authors:
 *   Daoan <daoan@taobao.com>
 *
 */
#ifndef TABLE_BUILDER3_H
#define TABLE_BUILDER3_H
#include "table_builder.hpp"
#include "table_builder1.hpp"
#include "table_builder2.hpp"

namespace tair
{
  namespace config_server
  {
    class table_builder3 : public table_builder
    {
      public:
        table_builder3(float change_ratio, uint32_t bucket_c, uint32_t copy_c)
          : table_builder(bucket_c, copy_c),
            stat_change_ratio(change_ratio),
            real_table_builder(NULL)
        {
        }

        ~table_builder3()
        {
          if (NULL != real_table_builder)
          {
            delete real_table_builder;
            real_table_builder = NULL;
          }
        }

      public:
        void set_available_server(const std::set<node_info*>& ava_ser);
        int is_this_node_OK(server_id_type node_id, int line_num,
            size_t node_idx, hash_table_type& hash_table_dest,
            int option_level, bool node_in_use = false);
        void caculate_capable();
        int get_tokens_per_node(const server_id_type& node_id);
        int rebuild_table(const hash_table_type& hash_table_source,
            hash_table_type& hash_table_dest,
            bool no_quick_table = false);

      private:
        float stat_change_ratio;
        table_builder* real_table_builder;
    };
  }
}
#endif
