#include <bits/stdc++.h>
#include "abc_api.h"


using namespace std;
using namespace abc;


int main(int argc, char * argv[])
{
    string fileName = "data/blif/c432.blif";
    Abc_Start();
    Abc_Frame_t * pAbc = Abc_FrameGetGlobalFrame();
    string command = "read data/genlib/mcnc.genlib";
    assert( Cmd_CommandExecute(pAbc, command.c_str()) == 0 );
    command = "read " + fileName;
    assert( Cmd_CommandExecute(pAbc, command.c_str()) == 0 );
    command = "print_gates";
    assert( Cmd_CommandExecute(pAbc, command.c_str()) == 0 );
    command = "print_delay";
    assert( Cmd_CommandExecute(pAbc, command.c_str()) == 0 );
    Abc_Stop();
    return 0;
}
