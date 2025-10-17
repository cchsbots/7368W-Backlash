//This is the menu file, which creates a menu on the controller screen to select which autonomous to run. From here, you can register different autonomous functions, and then select which one to run on the controller screen before a match begins. If you are just practicing, you can also select to run the driver control period from here.
#include "vex.h"
#include "menu.h"

namespace cchs
{
  menu::menu(vex::competition &competition, vex::controller &controller)
      : menuCompetition(competition), menuController(controller)
  {
    menuThread = vex::thread(menu::loop, static_cast<void *>(this));
  }

  int menu::loop(void *arg)
  {
    menu *mi = static_cast<menu *>(arg);
    auto ctlr = mi->menuController;
    auto comp = mi->menuCompetition;
    int step = 1;
    int btnA = 0;
    ctlr.Screen.clearScreen();
    while (true)
    {
      if (!mi->isComplete)
      {
        if (step == 1)
        {
          if (ctlr.ButtonDown.pressing())
          {
            mi->currentAutonIndex++;
            if (mi->currentAutonIndex >= mi->autons.size())
            {
              mi->currentAutonIndex = 0;
            }
            mi->currentAuton = mi->autons[mi->currentAutonIndex].auton;
          }
          else if (ctlr.ButtonUp.pressing())
          {
            mi->currentAutonIndex--;
            if (mi->currentAutonIndex < 0)
            {
              mi->currentAutonIndex = mi->autons.size() - 1;
            }
            mi->currentAuton = mi->autons[mi->currentAutonIndex].auton;
          }
          else if (ctlr.ButtonA.pressing())
          {
            btnA++;
            step = 2;
          }
          ctlr.Screen.setCursor(1, 1);
          ctlr.Screen.print("Auton: (up/down)           ");
          ctlr.Screen.setCursor(2, 1);
          ctlr.Screen.print("%s                         ", mi->autons[mi->currentAutonIndex].name);
          ctlr.Screen.setCursor(3, 1);
          ctlr.Screen.print("Press A to select          ");
        }
        else if (step == 2)
        {
          if (ctlr.ButtonA.pressing())
          {
            if (btnA == 0)
            {
              mi->autonMode = true;
              mi->isComplete = true;
            }
            btnA++;
          }
          else if (ctlr.ButtonB.pressing())
          {
            mi->autonMode = false;
            mi->isComplete = true;
          }
          else if (comp.isCompetitionSwitch() || comp.isFieldControl())
          {
            mi->isComplete = true;
          }
          else
          {
            if (btnA > 1)
              btnA = 0;
          }
          ctlr.Screen.setCursor(1, 1);
          ctlr.Screen.print("Plug in or choose mode      ");
          ctlr.Screen.setCursor(2, 1);
          ctlr.Screen.print("A - Auton, B - Driver       ");
          ctlr.Screen.setCursor(3, 1);
          ctlr.Screen.print("                            ");
        }
      }
      else
      {
        ctlr.Screen.setCursor(1, 1);
        ctlr.Screen.print("Ready                       ");
        ctlr.Screen.setCursor(2, 1);
        ctlr.Screen.print("                            ");
      }
      vex::this_thread::sleep_for(100);
    }
  }

  void menu::registerAuton(const char *name, fp auton)
  {
    autons.push_back({name, auton});
    if (!currentAuton)
    {
      currentAuton = auton;
    }
  }

  void menu::setCurrentAuton(const char *name)
  {
    for (int i = 0; i < autons.size(); i++)
    {
      if (strcmp(autons[i].name, name) == 0)
      {
        currentAuton = autons[i].auton;
        currentAutonIndex = i;
        break;
      }
    }
  }
}