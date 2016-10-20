
//===-- storage/Index.h - Index class type ----------------*- C++ -*-===//
//
//                     CAESR Graph Database 
//
// TODO: LICENSE
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This is the main class for Index structure on graphs.
///
//===----------------------------------------------------------------------===//

#ifndef _INDEX_H_
#define _INDEX_H_

#include "FileTypes.h"

#include <unordered_map>
#include <vector>

class Index {
public:
  bool  getVertexIndex();
  bool  getEdgeIndex();
  bool  getIndex();
private:
};

#endif /*_INDEX_H_*/
