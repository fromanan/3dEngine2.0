#include "pch.h"

#include "Engine.h"
#include "Backend.h"

int main(int argc, char* argv[]) {
	Backend::init();
	Engine::Run();
	return 0;
}
