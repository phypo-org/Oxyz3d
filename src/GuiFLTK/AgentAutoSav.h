#ifndef H__AgentAutoSav__H
#define H__AgentAutoSav__H



#include "Utils/PPAgent.h"



namespace M3d {

  //***************************
  class AgentAutoSav: public PPu::PPAgent{
  public:
    AgentAutoSav() :PPAgent("AutoSav") {}
  private:
    virtual bool work     () override;

  };
  //***************************

} // namespace

#endif
