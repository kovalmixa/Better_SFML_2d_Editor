#include "controller.h"

Controller* Controller::instance_ = nullptr;

Controller::Controller()
{
	//make definition of handlers here
}

Controller* Controller::get_instance()
{
	return instance_ ? instance_ : (instance_ = new Controller());
}
