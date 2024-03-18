#include "cmdline.h"
#include "headers.h"
#include "sasimi.h"

using namespace std;
using namespace std::filesystem;
using namespace cmdline;

#include <chrono>

void TimeSimulation(Abc_Ntk_t* pNtk, string inputFile, int nFrame) {
  Simulator_t smlt(pNtk, nFrame);
  smlt.Input(0);
  auto start = chrono::high_resolution_clock::now();
  smlt.Simulate();
  auto stop = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(stop - start);
  std::cout << "time taken for simulation: " << time.count() << std::endl;
}

parser Cmdline_Parser(int argc, char *argv[]) {
  parser option;
  option.add<string>("circuit", 'c', "original circuit file", false,
                     "data/su/c880.blif");
  option.add<string>("output", 'o', "path to output circuit files", false,
                     "appNtk/");
  option.add<string>("library", 'l', "standard cell library", false,
                     "data/library/mcnc.genlib");
  option.add<string>("metricType", '\0', "error metric type, er, nmed", false,
                     "er");
  option.add<float>("errorBound", 'e', "error metric upper bound", false, 0.05,
                    range(0.0, 1.0));
  option.add<int>("frameNumber", 'f', "simulation round", false, 100000,
                  range(1, INT_MAX));
  option.add<int>("maxLevel", '\0', "max TFO cut level", false, INT_MAX,
                  range(1, INT_MAX));
  option.add<string>("input", 'i', "path to file with inputs for simulation",
                     false, "");
  option.parse_check(argc, argv);
  return option;
}

int main(int argc, char *argv[]) {
  // command line parser
  parser option = Cmdline_Parser(argc, argv);
  string circuit = option.get<string>("circuit");
  string output = option.get<string>("output");
  string library = option.get<string>("library");
  Metric_t metricType = (option.get<string>("metricType") == "er")
                            ? Metric_t::ER
                            : Metric_t::NMED;
  float errorBound = option.get<float>("errorBound");
  int frameNumber = option.get<int>("frameNumber");
  int maxLevel = option.get<int>("maxLevel");
  string inputFile = option.get<string>("input");

  // deal with IO
  path inputPath(circuit);
  DASSERT(exists(inputPath));
  path outputPath(output);
  create_directory(outputPath);
  path outPrefix(outputPath);
  outPrefix += inputPath.stem().string();

  // start abc
  Abc_Start();

  // read library and circuit
  Abc_Frame_t *pAbc = Abc_FrameGetGlobalFrame();
  ostringstream command("");
  command << "read " << library;
  cout << "abc command " << command.str() << endl;
  DASSERT(!Cmd_CommandExecute(pAbc, command.str().c_str()));
  command.str("");
  command << "read " << inputPath.string();
  cout << "abc command " << command.str() << endl;
  DASSERT(!Cmd_CommandExecute(pAbc, command.str().c_str()));
  command.str("");
  command << "print_stats";
  cout << "abc command " << command.str() << endl;
  DASSERT(!Cmd_CommandExecute(pAbc, command.str().c_str()));

  // sasimi + vecbee
  Abc_Ntk_t *pNtk = Abc_NtkDup(Abc_FrameReadNtk(pAbc));

   TimeSimulation(pNtk, inputFile, frameNumber); 
  // stop abc
  Abc_Stop();

  return 0;
}
