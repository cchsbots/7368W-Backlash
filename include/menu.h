#include <vector>
// #include "vex.h"

namespace cchs
{
  typedef void (*fp)();
  typedef struct menuAuton
  {
    const char *name;
    fp auton;
  } menuAuton;

  class menu
  {
  private:
    vex::competition menuCompetition;
    vex::controller menuController;
    vex::thread menuThread;
    std::vector<menuAuton> autons;
    static int loop(void *arg);
    int currentAutonIndex;

  public:
    menu(vex::competition &competition, vex::controller &controller);
    fp currentAuton;
    void registerAuton(const char *name, fp auton);
    void setCurrentAuton(const char *name);
    bool autonMode = false;
    bool isComplete = false;
  };
}