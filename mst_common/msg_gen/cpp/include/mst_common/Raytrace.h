/* Auto-generated by genmsg_cpp for file /home/robot/Documents/Jomegatron_IGVC/mst_common/msg/Raytrace.msg */
#ifndef MST_COMMON_MESSAGE_RAYTRACE_H
#define MST_COMMON_MESSAGE_RAYTRACE_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"


namespace mst_common
{
template <class ContainerAllocator>
struct Raytrace_ {
  typedef Raytrace_<ContainerAllocator> Type;

  Raytrace_()
  : ranges()
  , num_ranges(0)
  {
  }

  Raytrace_(const ContainerAllocator& _alloc)
  : ranges(_alloc)
  , num_ranges(0)
  {
  }

  typedef std::vector<double, typename ContainerAllocator::template rebind<double>::other >  _ranges_type;
  std::vector<double, typename ContainerAllocator::template rebind<double>::other >  ranges;

  typedef int32_t _num_ranges_type;
  int32_t num_ranges;


  ROS_DEPRECATED uint32_t get_ranges_size() const { return (uint32_t)ranges.size(); }
  ROS_DEPRECATED void set_ranges_size(uint32_t size) { ranges.resize((size_t)size); }
  ROS_DEPRECATED void get_ranges_vec(std::vector<double, typename ContainerAllocator::template rebind<double>::other > & vec) const { vec = this->ranges; }
  ROS_DEPRECATED void set_ranges_vec(const std::vector<double, typename ContainerAllocator::template rebind<double>::other > & vec) { this->ranges = vec; }
private:
  static const char* __s_getDataType_() { return "mst_common/Raytrace"; }
public:
  ROS_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROS_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "77ea022a1e6008d0edfcc5431496c9ff"; }
public:
  ROS_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROS_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "float64[]   ranges\n\
int32       num_ranges\n\
\n\
"; }
public:
  ROS_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, ranges);
    ros::serialization::serialize(stream, num_ranges);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, ranges);
    ros::serialization::deserialize(stream, num_ranges);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(ranges);
    size += ros::serialization::serializationLength(num_ranges);
    return size;
  }

  typedef boost::shared_ptr< ::mst_common::Raytrace_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::mst_common::Raytrace_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct Raytrace
typedef  ::mst_common::Raytrace_<std::allocator<void> > Raytrace;

typedef boost::shared_ptr< ::mst_common::Raytrace> RaytracePtr;
typedef boost::shared_ptr< ::mst_common::Raytrace const> RaytraceConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::mst_common::Raytrace_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::mst_common::Raytrace_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace mst_common

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::mst_common::Raytrace_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::mst_common::Raytrace_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::mst_common::Raytrace_<ContainerAllocator> > {
  static const char* value() 
  {
    return "77ea022a1e6008d0edfcc5431496c9ff";
  }

  static const char* value(const  ::mst_common::Raytrace_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x77ea022a1e6008d0ULL;
  static const uint64_t static_value2 = 0xedfcc5431496c9ffULL;
};

template<class ContainerAllocator>
struct DataType< ::mst_common::Raytrace_<ContainerAllocator> > {
  static const char* value() 
  {
    return "mst_common/Raytrace";
  }

  static const char* value(const  ::mst_common::Raytrace_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::mst_common::Raytrace_<ContainerAllocator> > {
  static const char* value() 
  {
    return "float64[]   ranges\n\
int32       num_ranges\n\
\n\
";
  }

  static const char* value(const  ::mst_common::Raytrace_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::mst_common::Raytrace_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.ranges);
    stream.next(m.num_ranges);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct Raytrace_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::mst_common::Raytrace_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::mst_common::Raytrace_<ContainerAllocator> & v) 
  {
    s << indent << "ranges[]" << std::endl;
    for (size_t i = 0; i < v.ranges.size(); ++i)
    {
      s << indent << "  ranges[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.ranges[i]);
    }
    s << indent << "num_ranges: ";
    Printer<int32_t>::stream(s, indent + "  ", v.num_ranges);
  }
};


} // namespace message_operations
} // namespace ros

#endif // MST_COMMON_MESSAGE_RAYTRACE_H

