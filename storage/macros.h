#ifndef _MACRO_H_
#define _MACRO_H_

#define MAGIC_SOURCE_NODE(a) \
  std::cout << "MAGIC_SOURCE_NODE: Source node address " << a << "\n";

#define MAGIC_PREFETCH_TRIGGER \
  std::cout << "MAGIC_PREFETCH_TRIGGER" << "\n";

#endif /* _MACRO_H_ */
