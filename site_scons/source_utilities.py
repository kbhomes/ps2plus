import glob
import os

def ExternalObject(env, root, path):
    modified_path = path

    while modified_path.startswith(root):
        modified_path = modified_path[len(root):]

    modified_path = '__external__' + modified_path

    input = env.Dir('.').srcnode().File(path)
    output = env.Dir('.').File(env.subst(modified_path + '$OBJSUFFIX'))
    return env.Object(source=input, target=output)

def GlobRecursive(env, pattern):
    # Get the source directory from the environment, to account for VariantDirs
    return glob.glob(
        env._canonicalize(env.subst(pattern)), 
        recursive=True,
        root_dir=env.fs.getcwd().srcdir.abspath
    )

def GlobRecursiveExternal(env, root, pattern):
    if not root.endswith('/') or not root.endswith('\\'):
        root += os.path.sep
        
    paths = GlobRecursive(env, os.path.join(root, pattern))
    return [ExternalObject(env, root, path) for path in paths]