/*
 * tcp_stream_management.h
 *
 * Created on: 06/10/2012
 *
 * =========================================================================
 *  Copyright (C) 2012-2013, Daniele De Sensi (d.desensi.software@gmail.com)
 *
 *  This file is part of Peafowl.
 *
 *  Peafowl is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.

 *  Peafowl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with Peafowl.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 * =========================================================================
 */

#ifndef TCP_STREAM_MANAGEMENT_H_
#define TCP_STREAM_MANAGEMENT_H_

#include <peafowl/peafowl.h>

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * If the sequence number of the received segment is 'x',
 * and the expected sequence number is 'y',
 * if x-y > PFWL_TCP_MAX_OUT_OF_ORDER_BYTES, the segment will
 * not be buffered.
 */
#define PFWL_TCP_MAX_OUT_OF_ORDER_BYTES 32768

enum pfwl_tcp_reordering_statuses {
  PFWL_TCP_REORDERING_STATUS_IN_ORDER = 0,
  PFWL_TCP_REORDERING_STATUS_OUT_OF_ORDER,
  PFWL_TCP_REORDERING_STATUS_REBUILT
};

typedef struct pfwl_tcp_reordering_reordered_segment {
  unsigned char* data;
  uint32_t data_length;
  uint8_t status : 2;
  uint8_t connection_terminated : 1;
} pfwl_tcp_reordering_reordered_segment_t;

/**
 * Deletes all the pendent fragments belonging to a certain flow.
 * @param victim
 */
void pfwl_reordering_tcp_delete_all_fragments(
    pfwl_tracking_informations_t* victim);

/**
 * Tracks the TCP connection.
 * @param pkt The informations about the packet.
 * @param tracking A pointer to the structure containing the information
 *                 about the TCP connection.
 * @return If the packet is out of order, if it has already been received
 *         or if an error occurred, the returned structure contains
 *         PFWL_TCP_REORDERING_STATUS_OUT_OF_ORDER in the 'status' field.
 *         The 'data' and 'data_length' fields in this case have no
 *         meaning.
 *
 * 		   If the data is in order but doesn't fill an 'hole' in the
 * 		   segment space, the returned structure contains
 * 		   PFWL_TCP_REORDERING_STATUS_IN_ORDER in the 'status' field.
 * 		   The 'data' and 'data_length' fields in this case have no
 * 		   meaning.
 *
 *         If the received data is in order and fills an 'hole' in
 *         the segment space, the returned structure contains
 *         PFWL_TCP_REORDERING_STATUS_REBUILT in the 'status' field.
 *         The 'data' field will contain a pointer to the new (longer)
 *         segment. This pointer must be freed after the use. The
 *         'data_length' field contains the length of the new (longer)
 *         segment.
 */
pfwl_tcp_reordering_reordered_segment_t pfwl_reordering_tcp_track_connection(
    pfwl_identification_result_t* pkt, pfwl_tracking_informations_t* tracking);

/**
 * Only checks if the connection terminates.
 * @param pkt The informations about the packet.
 * @param tracking A pointer to the structure containing the informations
 *                 about the TCP connection.
 * @return 1 if the connection is terminated, 0 otherwise.
 */
uint8_t pfwl_reordering_tcp_track_connection_light(
    pfwl_identification_result_t* pkt, pfwl_tracking_informations_t* tracking);

#ifdef __cplusplus
}
#endif

#endif /* TCP_STREAM_MANAGEMENT_H_ */
