#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/process.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;

bp::child Make(int time = 0, std::string build_type="Debug")
{
  std::string cmd("cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=");
  cmd += build_type;
  std::cout << cmd << '\n';
  bp::child c(cmd, bp::std_out > stdout);
  if (time) {
    if (!c.wait_for(std::chrono::seconds(time)));
      c.terminate();
  }
  else
    c.wait();
  return c;
}

bp::child Build(int time = 0)
{
  std::string cmd("cmake --build _builds");

  std::cout << cmd << '\n';
  bp::child c(cmd, bp::std_out > stdout);
  if (time){
    if (!c.wait_for(std::chrono::seconds(time)));
      c.terminate();
  }
  else
    c.wait();
  return c;
}

bp::child Target(std::string inst)
{
  std::string cmd("cmake --build _builds --target ");
  cmd +=inst;

  std::cout << cmd << '\n';
  bp::child c(cmd, bp::std_out > stdout);
  c.wait();
  return c;
}

int main(int argc, char *argv[])
{
    int result;

    po::options_description desc("Usage: builder [options]\nAllowed options");
    desc.add_options()
      ("help", "выводим вспомогательное сообщение")
      ("config", po::value<std::string>(), "указываем конфигурацию сборки (по умолчанию Debug)")
      ("install", "добавляем этап установки (в директорию _install)")
      ("pack", "добавляем этап упакови (в архив формата tar.gz)")
      ("timeout", po::value<int>(), "указываем время ожидания (в секундах)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
      std::cout << desc << '\n';
      return 1;
    }
    else if (vm.count("config"))
    {
      std::string par(vm["config"].as<std::string>());
      try{
      result = Make(0, par).exit_code();
      }
      catch(...){result = Make().exit_code();}
      if (!result)
        result = Build().exit_code();
    }
    else if (vm.count("install") && vm.count("pack"))
    {
      result = Make().exit_code();
      if (!result)
        result = Build().exit_code();
      if (!result)
        result = Target("install").exit_code();
        if (!result)
          result = Target("pack").exit_code();
    }
    else if (vm.count("install"))
    {
      result = Make().exit_code();
      if (!result)
        result = Build().exit_code();
      if (!result)
        result = Target("install").exit_code();
    }
    else if (vm.count("pack"))
    {
      result = Make().exit_code();
      if (!result)
        result = Build().exit_code();
      if (!result)
        result = Target("package").exit_code();
    }
    else if (vm.count("timeout"))
    {
      int time = vm["timeout"].as<int>();
      
      result = Make(time).exit_code();
      if (!result)
        result = Build(time).exit_code();
    }
    else {
      result = Make().exit_code();
      if (!result)
        result = Build().exit_code();
    }
}
