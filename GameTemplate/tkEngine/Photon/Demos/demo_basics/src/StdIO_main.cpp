#include "BasicsApplication.h"
#include "StdIO_UIListener.h"

int main(void)
{
	BasicsApplication::run(new StdIO_UIListener());
}