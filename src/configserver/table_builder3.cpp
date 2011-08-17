/*
 * (C) 2007-2010 Alibaba Group Holding Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * table_builder3.cpp is the implement of table build strategy which make the data safety
 * is the first priority
 *
 * Version: $Id: table_builder3.cpp 183 2011-08-11 02:02:42Z zongdai@taobao.com $
 *
 * Authors:
 *   Daoan <daoan@taobao.com>
 *
 */
#include "table_builder3.hpp"
#include <stdio.h>
#include <stdlib.h>

namespace tair
{
  namespace config_server
  {
    void table_builder3::set_available_server(const set<node_info*>& ava_server)
    {
      //choose which strategy to select
      set<uint32_t> room;
      room.clear();
      set<uint32_t>::const_iterator room_iter;
      for (set<node_info*>::const_iterator it = ava_server.begin();
          it != ava_server.end(); ++it)
      {
        uint32_t ri = ((*it)->server->server_id & pos_mask);
	log_error("machine %d groupid %d posmask %d",(*it)->server->server_id,ri,pos_mask);
        if ((room_iter = room.find(ri)) == room.end())
          room.insert(ri);
      }

      if (NULL != real_table_builder)
      {
        delete real_table_builder;
        real_table_builder = NULL;
      }
	
	log_error("room size is %d",room.size());

      if (1 == room.size())
      {
        real_table_builder = new table_builder1(bucket_count, copy_count);
      }
      else
      {
        real_table_builder = new table_builder2(stat_change_ratio, bucket_count, copy_count);
      }
      real_table_builder->set_pos_mask(pos_mask);
      return real_table_builder->set_available_server(ava_server);
    }

    int table_builder3::is_this_node_OK(server_id_type node_id, int line_num,
        size_t node_idx,
        hash_table_type & hash_table_dest,
        int option_level, bool node_in_use)
    {
      if (NULL == real_table_builder)
      {
        log_error("real_table_builder is not init");
        // assert it now
        assert(false);
      }

      return real_table_builder->is_this_node_OK(node_id, line_num, node_idx,
          hash_table_dest, option_level, node_in_use);
    }

    void table_builder3::caculate_capable()
    {
      if (NULL == real_table_builder)
      {
        log_error("real_table_builder is not init");
        // assert it now
        assert(false);
      }

      return real_table_builder->caculate_capable();
    }

    int table_builder3::get_tokens_per_node(const server_id_type & node_id)
    {
      if (NULL == real_table_builder)
      {
        log_error("real_table_builder is not init");
        // assert it now
        assert(false);
      }

      return real_table_builder->get_tokens_per_node(node_id);
    }

    int table_builder3::
      rebuild_table(const hash_table_type& hash_table_source,
          hash_table_type& hash_table_dest, bool no_quick_table)
      {
        if (NULL == real_table_builder)
        {
          log_error("real_table_builder is not init");
          // assert it now
          assert(false);
        }

        return real_table_builder->rebuild_table(hash_table_source, hash_table_dest, no_quick_table);
      }

  }
}
