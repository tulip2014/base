
def add(a, b):
    '''test add'''
    return a + b;

def fabla(a):
    if a == 0 :
        return 0;
    return fabla(a-1) + a;


a = "asf";
print((fabla(100)))
