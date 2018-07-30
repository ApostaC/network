#ifndef _WORKER_HH_
#define _WORKER_HH_

/**
 * Class Worker
 *
 * Worker reads input packets from a buffer, 
 * writes it's output to a file without modifing input.
 *
 * It outputs:
 * 1.Some basic info
 *      number, time, srcip, dstip, srcip, dstip, protocol-type, 
 *      srcport, dstport, ack-seq, seq, is_syn, is_ack, is_fin,
 *      is_rst, mptcp_option, window_size, len, hdr_len,
 * 2.Analyzed info
 *      lost_segment, ack_rtt, retransmission, fast_retransmission,
 *      spurious_retransmission, BIF
 */

class Worker
{
};





#endif
