
// N = 0, ..., 5
template <typename F, typename T1, ..., typename TN>
void remote_call(procid_t target, F remote_function, T1 &i1, ..., TN &iN) {
  ASSERT_LT(target, senders.size());
  dc_impl::remote_call_issueN<F, T1, ..., TN>::exec(
      senders[target], STANDARD_CALL, target, remote_function, i1, ..., iN);
}

// N = 0, ..., 5
template <typename F, typename T1, ..., typename TN>
void reply_remote_call(procid_t target, F remote_function, T1 &i1, ...,
                       TN &iN) {
  ASSERT_LT(target, senders.size());
  dc_impl::remote_call_issueN<F, T1, ..., TN>::exec(
      senders[target], STANDARD_CALL | FLUSH_PACKET, target, remote_function,
      i1, ..., iN);
}

// N = 0, ..., 5
template <typename F, typename T1, ..., typename TN>
void control_call(procid_t target, F remote_function, T1 &i1, ..., TN &iN) {
  ASSERT_LT(target, senders.size());
  dc_impl::remote_call_issueN<F, T1, ..., TN>::exec(
      senders[target], STANDARD_CALL | CONTROL_PACKET, target, remote_function,
      i1, ..., iN);
}

// N = 0, ..., 5
template <typename Iterator, typename F, typename T1, ..., typename TN>
void remote_call(Iterator target_begin, Iterator target_end, F remote_function,
                 T1 &i1, ..., TN &iN) {
  if (target_begin == target_end) return;
  dc_impl::remote_broadcast_issueN<Iterator, F, T1, ..., TN>::exec(
      senders, STANDARD_CALL, target_begin, target_end, i1, ..., iN);
}

// N = 0, ..., 6
template <typename F, typename T1, ..., typename TN>
void custom_remote_request(procid_t target, size_t handle, unsigned char flags,
                           F remote_function, T1 &i1, ..., TN &iN) {
  dc_impl::remote_request_issueN<F, T1, ..., TN>::exec(
      senders[target], handle, flags, target, remote_function, i1, ..., iN);
}

// N = 0, ..., 6
template <typename F, typename T1, ..., typename TN>
typename dc_impl::function_ret_type<__GLRPC_FRESULT>::type remote_request(
    procid_t target, F remote_function, T1 &i1, ..., TN &iN) {
  request_future<__GLRPC_FRESULT> reply;
  custom_remote_request(target, reply.get_handle(),
                        STANDARD_CALL | FLUSH_PACKET, remote_function, i1, ...,
                        iN);
  return reply();
}

// N = 0, ..., 6
template <typename F, typename T1, ..., typename TN>
request_funture<__GLRPC_FRESULT> future_remote_request(procid_t target,
                                                       F remote_function,
                                                       T1 &i1, ..., TN &iN) {
  ASSERT_LT(target, senders.size());
  request_future<__GLRPC_FRESULT> reply;
  custom_remote_request(target, reply.get_handle(), STANDARD_CALL,
                        remote_function, i1, ..., iN);
  return reply;
}
