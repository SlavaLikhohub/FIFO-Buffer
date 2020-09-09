import py_fifo_buff as F

def print_div(N = 30):
    print("-" * N)
    print()
    

max_size = 50
ell = list(range(50))

buff = F.FIFO_Buffer_py(max_size, verbouse = 1)


buff.add_elements(ell)
print ("Added elements")
buff.print()
print_div()

ell_r = []
ell_r.append(buff.read_elements(20))
print(f"Read element {ell_r}")
buff.print()
print_div()


ell = list(range(30))
buff.add_elements(ell)
print ("Added elements")
buff.print()
print_div()
