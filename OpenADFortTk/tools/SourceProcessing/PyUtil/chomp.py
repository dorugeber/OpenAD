def chomp(s):
    if len(s) == 0:
        return s

    if s[-1] == '\n':
        return s[:-1]

    return s
