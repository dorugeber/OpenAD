// $Id: gmpi_impl.hpp,v 1.3 2004/02/22 18:44:46 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


namespace GMPI {

  template <class Comm_ref_t>
  void Comm::Send (const Comm_ref_t& data, int dest, int tag) const {
    typedef typename Comm_ref_t::base_t   base_t;
    buffer_t<base_t>                      buffer;
    buffer << data.object_ref();
    // size_t bsize= buffer.size();
    // my_comm.Bsend (&bsize, 1, mpi_size_t, dest, tag); // at first send buffer size
    my_comm.Send (buffer.address(), buffer.size(), buffer.mpi_t, dest, tag);
  }

  template <class Comm_ref_t>
  void Comm::Recv (Comm_ref_t& data, int source, int tag, MPI::Status& status) const {
    typedef typename Comm_ref_t::base_t   base_t;
    // size_t                                bsize;
    // my_comm.Recv (&bsize, 1, mpi_size_t, source, tag, status); // at first receive buffer size
    // MPI::Status                           status;
    my_comm.Probe (source, tag, status);
    buffer_t<base_t>                      buffer;
    size_t                                bsize= status.Get_count (buffer.mpi_t);
    buffer.reserve (bsize);
    // if wildcards are used make sure to get the second message from the same processor
    // source= status.Get_source (); tag= status.Get_tag (); 
    my_comm.Recv (buffer.address(), bsize, buffer.mpi_t, source, tag, status);
  }

  template <class Comm_ref_t>
  void Intracomm::Bcast (Comm_ref_t& data, int root) const {
    typedef typename Comm_ref_t::base_t   base_t;
    buffer_t<base_t>                      buffer;
    size_t                                bsize;
    if (Get_rank () == root) {
      buffer << data.object_ref(); bsize= buffer.size(); }
    my_comm.Bcast (&bsize, 1, mpi_size_t, root);
    if (Get_rank () != root) 
      buffer.reserve (bsize);
    my_comm.Bcast (buffer.address(), bsize, buffer.mpi_t, root);
    if (Get_rank () != root) 
      buffer >> data.object_ref();
  }

} // namespace GMPI
