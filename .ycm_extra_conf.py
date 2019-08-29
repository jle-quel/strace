def Settings( **kwargs ):
    return {
        'flags': [
            '-x',
            'c',
            '-Wall',
            '-Wextra',
            '-Werror',
            '-I', '/home/vagrant/ft_strace/inc',
            '-I', '/usr/include',
        ],
    }
