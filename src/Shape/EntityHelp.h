#ifndef H__ENTITY_HELP__H
#define H__ENTITY_HELP__H

namespace PP3d {

  //**************************************
  class EntityHelp{
  public:
    static bool AsCommonPoint( LinePtr A, LinePtr B )
    {
      return A->first() == B->first()
	|| A->first() == B->second()
	||  A->second() == B->first()
	||  A->second() == B->second();
    }
  };
  //**************************************

}
#endif
