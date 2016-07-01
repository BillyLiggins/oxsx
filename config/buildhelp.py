from dependency   import Dependency, VALID_FIELDS
from ConfigParser import ConfigParser, NoOptionError
from SCons.Script import Split, Glob, Copy, File, Dir, Exit
from platform import system
from subprocess import check_call
import os

def read_dependencies(filename):
    cparse = ConfigParser()
    cparse.read(filename)
    
    dependencies = {}
    for dep_name in cparse.sections():
        try:
            libs   = cparse.get(dep_name, "libs")
            cheads = cparse.get(dep_name, "check_headers")
        except NoOptionError:
            print "Incomplete dependency spec for {0}, (needs libs & check_headers)".format(dep_name)
            raise
        dependencies[dep_name] = Dependency(dep_name, libs, cheads)
    return dependencies

def check_dependency(conf, dependency):
    print "\nChecking {0} dependencies..".format(dependency.name)
    for header in Split(dependency.check_headers):
        if not conf.CheckCXXHeader(header):
            print('!! Cannot locate header {0} ...'.format(header))
            Exit(0)
                
    # check libs
    for lib in Split(dependency.libs):
        if not conf.CheckLib(lib):
            print('!! Cannot locate library {0}'.format(lib))
            Exit(0)


def sanity_checks(conf):
    if not conf.CheckCXX():
        print('!! C++ compiler is not available...')
        Exit(0)


def parse_user_config(filename, dependencies):
    '''
    Read user_config and update the relevant fields
    '''
    compiler = None # Use default
    cparse = ConfigParser()
    cparse.read(filename)
    for dep_name in cparse.sections():

        if dep_name == "g++":
            try:
                compiler = cparse.get(dep_name, 'binary_path')
                if not os.basename(compiler) == "g++":
                    print('!! user_config specifies something other than full path to g++')
                    Exit(0)
            except:
                # Assume we don't want to set the g++ compiler
                pass

        elif dep_name in dependencies.keys():
            # loop over options for that dependency
            for opt in cparse.options(dep_name):
                if opt in VALID_FIELDS:
                    dependencies[dep_name].__setattr__(opt, cparse.get(dep_name, opt))
                else:
                    print "Unknown build option: {0} for dependency {1}".format(opt, dep_name)
    
            # should be installed in standard location - try to find it
            if dependencies[dep_name].lib_path is None:
                try:
                    dependencies[dep_name].lib_path = find_library(dependencies[dep_name].libs.split()[0])
                except ValueError:
                    # hmm.
                    pass
    
        else:
            print('!! user_config specifies unknown dependency "{0}"'.format(dep_name))
            Exit(0)
    return compiler

def update_and_check_env(conf, dependencies):
    '''
    Update the build environment with the dependencies
    '''
    # First check that C++11 support is enabled
    for dep in dependencies.values():
        if dep.header_path:
            conf.env.Append(CPPPATH = [dep.header_path])
        if dep.flags:
            conf.env.Append(CFLAGS = [dep.flags])
        if dep.libs:
            conf.env.Append(LIBS = Split(dep.libs))
        if dep.lib_path:
            conf.env.Append(LIBPATH = [dep.lib_path])
            conf.env.Append(RPATH   = [dep.lib_path])
            if conf.env["SYSTEM"] == "Darwin":
                conf.env.Append(LINKFLAGS = "-rpath {0}".format(dep.lib_path))
        check_dependency(conf, dep)
    

def create_cpy_commands(conf, dependencies, dep_name, copy_folder):
    '''
    Create os dependent commands. On darwin: copy gsl libs, fix
    the install names for dylibs using install_name_tool, and 
    replace lib path with the patched version. On linux: do nothing
    '''
    if conf.env["SYSTEM"] == "Darwin" and dependencies[dep_name].lib_path:
        lib_path = dependencies[dep_name].lib_path
        commands = []

        for lib in Glob(os.path.join(lib_path, "*{0}*".format(dep_name))):
            new_path = os.path.join(copy_folder, 
                                    os.path.basename(lib.rstr()))
            action = [Copy("$TARGET", "$SOURCE")]
            
            if "dylib" in lib.rstr() and not os.path.islink(lib.rstr()):
                action += [fix_dylib_for_darwin]

            kw = {
                'target' : '{0}'.format(new_path),
                'source' : '{0}'.format(lib),
                'action' : action
                }
            commands.append(kw)

        dependencies[dep_name].lib_path = Dir(copy_folder).abspath
        return commands

    else:
        return []

def fix_dylib_for_darwin(target, source, env):
    '''
    Fix the the install_names for darwin for all dylibs in target
    (here they are set to the abspath of the library)
    '''
    for t in target:
        abspath = File(t).abspath
        check_call("install_name_tool -id {0} {0}".format(abspath),
                   shell = True)

def find_library(library_name):
    """
    Look for a library in the standard install locations, return None if not found
    """
    for path in (os.path.join(os.environ["HOME"],"lib"), "/usr/lib", "/usr/local/lib"):
        for libext in ("a", "dylib", "so"):
            file_name = "lib{0}.{1}".format(library_name, libext)
            try:
                if os.path.exists(os.path.join(path, file_name)):
                    return path
            except:
                pass
    raise ValueError


def test_cpp11():
    env.Default
