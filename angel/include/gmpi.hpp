// $Id: gmpi.hpp,v 1.4 2004/02/22 18:44:46 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifndef 	_gmpi_include_
#define 	_gmpi_include_


#include "mpi.h"
#include <utility>
#include <vector>
#include <list>
#include <deque>
// #include <complex>


namespace GMPI {

  using std::pair; using std::vector; using std::list; using std::deque;

  /// Returns MPI data type for all available base types
  inline MPI::Datatype which_mpi_t (char) {return MPI::CHAR;}
  // inline MPI::Datatype which_mpi_t (wchar_t) {return MPI::WCHAR;}
  // inline MPI::Datatype which_mpi_t (signed char) {return MPI::SIGNED_CHAR;}
  inline MPI::Datatype which_mpi_t (signed short) {return MPI::SHORT;}
  inline MPI::Datatype which_mpi_t (signed int) {return MPI::INT;}
  inline MPI::Datatype which_mpi_t (signed long) {return MPI::LONG;}
  inline MPI::Datatype which_mpi_t (unsigned char) {return MPI::UNSIGNED_CHAR;}
  inline MPI::Datatype which_mpi_t (unsigned short) {return MPI::UNSIGNED_SHORT;}
  inline MPI::Datatype which_mpi_t (unsigned) {return MPI::UNSIGNED;}
  // inline MPI::Datatype which_mpi_t (unsigned int) {return MPI::UNSIGNED_INT;}
  inline MPI::Datatype which_mpi_t (unsigned long) {return MPI::UNSIGNED_LONG;}
  inline MPI::Datatype which_mpi_t (double) {return MPI::DOUBLE;}
  inline MPI::Datatype which_mpi_t (long double) {return MPI::LONG_DOUBLE;}
  // inline MPI::Datatype which_mpi_t (bool) {return MPI::BOOL;}
  // inline MPI::Datatype which_mpi_t (complex<float>) {return MPI::COMPLEX;}
  // inline MPI::Datatype which_mpi_t (complex<double>) {return MPI::DOUBLE_COMPLEX;}
  // inline MPI::Datatype which_mpi_t (complex<long double>) {return MPI::LONG_DOUBLE_COMPLEX;}
  inline MPI::Datatype which_mpi_t (pair<int,int>) {return MPI::TWOINT;}
  inline MPI::Datatype which_mpi_t (pair<float,int>) {return MPI::FLOAT_INT;}
  inline MPI::Datatype which_mpi_t (pair<double,int>) {return MPI::DOUBLE_INT;}
  inline MPI::Datatype which_mpi_t (pair<long double,int>) {return MPI::LONG_DOUBLE_INT;}
  inline MPI::Datatype which_mpi_t (pair<short,int>) {return MPI::SHORT_INT;}

  /// MPI data type to communicate sizes
  const MPI::Datatype mpi_size_t= which_mpi_t (size_t());

  template <typename Base_t>
  class buffer_t {
  public:
    typedef Base_t       base_t;      ///< The base type of the buffer
    MPI::Datatype        mpi_t;       ///< MPI data type used in operations
    const int            base_t_size; ///< Base type's size in byte
  private:
    vector<Base_t>       my_buffer;   // Vector, to use reallocation and copy
    mutable size_t       write_pos,   // Written entries (of Base_t)
                         read_pos;    // Read entries (of Base_t)
    mutable bool         read_write;  // Either read or write, true == read
  public:
    /// Set an empty configuration
    buffer_t () :  mpi_t (which_mpi_t (Base_t())), 
                base_t_size (sizeof (Base_t)),
		write_pos (0), read_pos (0), read_write (false) {}
    /// Reserve \p n entries of base type at the beginning
    buffer_t (size_t n) :  mpi_t (which_mpi_t (Base_t())), 
                base_t_size (sizeof (Base_t)),
		write_pos (0), read_pos (0), read_write (false) {reserve (n);}

    /// Reserve memory for another \p n entries of base type
    void reserve (size_t n) {
      if (read_write) { // change to write
	write_pos= 0; my_buffer.resize (n); read_write= false; return; }
      if (write_pos+n > my_buffer.size()) my_buffer.resize (write_pos+n);}

    /// Whether all data is read
    bool empty () const {
      return read_pos == write_pos; }

    /// The number of remaining entries to read
    size_t remaining () const {
      return write_pos - read_pos; }

    /// Read one entry of base type from buffer
    Base_t read () const {
      if (!read_write) { // change to read
	read_pos= 0; read_write= true; }
      // if (read_pos >= write_pos) throw gmpi_exception ("Read past end");
      return my_buffer[read_pos++]; }

    /// Undo effect of last reading
    void unread () const {
      // if (!read_write) throw gmpi_exception ("Unread in write mode");
      // if (read_pos == 0) throw gmpi_exception ("Unread at the beginning");
      read_pos--; }
      
    /// Write one entry of base type into buffer
    void write (Base_t output) {
      if (read_write) { // change to write
	write_pos= 0; my_buffer.resize (1); read_write= false; }
      // if (write_pos > my_buffer.size()) throw gmpi_exception ("Written past end");
      reserve (1);
      my_buffer[write_pos++]= output; }

    /// Load \p n entries from the buffer into \p address
    void load (Base_t* address, size_t n) const {
      if (!read_write) { // change to read
	read_pos= 0; read_write= true; }
      // if (read_pos+n > write_pos) throw gmpi_exception ("Read past end");
      memcpy (address, &my_buffer[read_pos], n * base_t_size);
      read_pos+= n; }

    /// Store \p n entries from \p address into the buffer
    void store (Base_t* address, size_t n) {
      if (read_write) { // change to write
	write_pos= 0; my_buffer.resize (n); read_write= false; }
      reserve (write_pos+n - my_buffer.size());
      memcpy (&my_buffer[write_pos], address, n * base_t_size);
      write_pos+= n; }

    /// Buffer size, e.g. to send or receive data
    size_t size () const {return my_buffer.size(); }

    /// Buffer address (as non-const), e.g. to receive data
    Base_t* address () {return &my_buffer[0]; }

    /// Buffer address (as const), e.g. to send data
    const Base_t* buffer_address () const {return &my_buffer[0]; }

    /// Explicit memory release
    void free () {
      my_buffer.resize (0); write_pos= read_pos= 0; read_write= false; }      
  };

  /// Read one entry of buffer's base type
  template <typename Base_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, Base_t& input) {
    input= buffer.read (); return buffer; }

  /// Write one entry of buffer's base type
  template <typename Base_t> inline
  buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, const Base_t& output) {
    buffer.write (output); return buffer; }

  /// Read a vector of buffer's base type in faster mode than arbitrary vectors
  template <typename Base_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, 
				      vector<Base_t>& input) {
    int csize= input.size(), n= buffer.remaining();
    input.resize (csize + n);
    buffer.load (&input[csize], n); 
    return buffer; }
  
  /// Write a vector of buffer's base type in faster mode than arbitrary vectors
  template <typename Base_t> inline
  const buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, 
				      const vector<Base_t>& output) {
    int n= output.size();
    buffer.store (&output[0], n); 
    return buffer; }

  // =========== Derived Operators ============================================
  
  /// Reads a pair of arbitrary types
  template <typename Base_t, typename Scalar1_t, typename Scalar2_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, 
				      pair<Scalar1_t, Scalar2_t>& input) {
    buffer >> input.first >> input.second; return buffer; }

  /// Writes a pair of arbitrary types
  template <typename Base_t, typename Scalar1_t, typename Scalar2_t> inline
  buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, 
				      const pair<Scalar1_t, Scalar2_t>& output) {
    buffer << output.first << output.second; return buffer; }

  /// Reads a vector of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, 
				      vector<Scalar_t>& input) {
    size_t size; buffer >> size;
    for (size_t c= 0; c < size; c++) {
      Scalar_t scalar; buffer >> scalar; input.push_back (scalar); }
    return buffer; }

  /// Writes (appends to) a vector of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, 
				const vector<Scalar_t>& output) {
    buffer << output.size(); // Base_t must be large enough for the vector size
    for (typename vector<Scalar_t>::const_iterator it= output.begin(), end= output.end(); 
	 it != end;) buffer << *it++;
    return buffer; }

  /// Reads a list of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, 
				      list<Scalar_t>& input) {
    size_t size; buffer >> size;
    for (size_t c= 0; c < size; c++) {
      Scalar_t scalar; buffer >> scalar; input.push_back (scalar); }
    return buffer; }

  /// Writes (appends to) a list of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, 
				const list<Scalar_t>& output) {
    buffer << output.size(); // Base_t must be large enough for the list size
    for (typename list<Scalar_t>::const_iterator it= output.begin(), end= output.end(); it != end;)
      buffer << *it++;
    return buffer; }

  /// Reads a deque of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  const buffer_t<Base_t>& operator>> (const buffer_t<Base_t>& buffer, 
				      deque<Scalar_t>& input) {
    size_t size; buffer >> size;
    for (size_t c= 0; c < size; c++) {
      Scalar_t scalar; buffer >> scalar; input.push_back (scalar); }
    return buffer; }

  /// Writes (appends to) a deque of an arbitrary type
  template <typename Base_t, typename Scalar_t> inline
  buffer_t<Base_t>& operator<< (buffer_t<Base_t>& buffer, 
				const deque<Scalar_t>& output) {
    buffer << output.size(); // Base_t must be large enough for the deque size
    for (typename deque<Scalar_t>::const_iterator it= output.begin(), end= output.end(); 
	 it != end;) buffer << *it++;
    return buffer; }

  template <typename Base_t, typename Object_t>
  class comm_ref_t {
  public:
    typedef Base_t                 base_t;
    typedef Object_t               object_t;
  private:
    object_t&                      my_object_ref;
  public:
    comm_ref_t (object_t& o) : my_object_ref (o) {}
    object_t& object_ref() {return my_object_ref; }
    const object_t& object_ref() const {return my_object_ref; }
  };

  class Comm {
  protected:
    MPI::Intracomm         my_comm;
  public:
    Comm (const MPI::Intracomm& mpi_comm) : my_comm (mpi_comm.Dup()) {}

    MPI::Intracomm& mpi_comm_ref () {return my_comm;}
    const MPI::Intracomm& mpi_comm_ref () const {return my_comm;}
   
    /// Send \p data to \p dest marked with \p tag
    template <typename Comm_ref_t>
    void Send (const Comm_ref_t& data, int dest, int tag) const;

    template <typename Comm_ref_t>
    void Recv (Comm_ref_t& data, int source, int tag, MPI::Status& status) const;

    int Get_size () const {return my_comm.Get_size (); }

    int Get_rank () const {return my_comm.Get_rank (); }
  };

  class Intracomm : public Comm {
  public:
    Intracomm (MPI::Intracomm& mpi_comm) : Comm (mpi_comm) {}
    
    void Barrier () const {my_comm.Barrier (); }

    template <typename Comm_ref_t>
    void Bcast (Comm_ref_t& data, int root) const;

    void Bcast (void* buffer, int count, const MPI::Datatype& datatype, int root) const {
      my_comm.Bcast (buffer, count, datatype, root); }

    template <typename Comm_ref_t>
    void Reduce (const Comm_ref_t& senddata, Comm_ref_t& recvdata, 
		 const MPI::Op& op, int root) const;

    template <typename Comm_ref_t>
    void Allreduce (const Comm_ref_t& senddata, Comm_ref_t& recvdata, 
		    const MPI::Op& op) const;    

    // template <typename Base_t>
    // void Allreduce (const Base_t& senddata, Base_t& recvdata, const MPI::Op& op) const {
    //   my_comm.Allreduce (&senddata, &recvdata, 1, which_mpi_t (senddata), op); }

    void Allreduce (const void* sendbuf, void* recvbuf, int count, 
		    const MPI::Datatype& datatype, const MPI::Op& op) const {
      my_comm.Allreduce (sendbuf, recvbuf, count, datatype, op); }

    void Gather (const void* sendbuf, int sendcount, const MPI::Datatype& sendtype,
		 void* recvbuf, int recvcount, const MPI::Datatype& recvtype, int root) const {
      my_comm.Gather (sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root); }
  };

} // namespace GMPI

#include "gmpi_impl.hpp" // long template implementations

#endif // 	_gmpi_include_






