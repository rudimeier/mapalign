import numpy as np
import embed
import sys


def random_matrix(n):
    tmp = np.random.randint(0,100, size=(n, n))
    for i in range(0, n):
        for j in range(0, i):
            tmp[i,j] = tmp[j,i]
    return tmp

n = int(sys.argv[1])

matrixfile = "matrix-%08d.dat" % (n)
outfile_embedding = "out-embedding-%08d.dat" % (n)
outfile_result = "out-result-%%s-%08d.dat" % (n)

#L = random_matrix(n)
#np.savetxt(matrixfile, L, delimiter='\t', fmt='%.2f')
L = np.loadtxt(matrixfile)

(embedding, result) = embed.compute_diffusion_map(L)
np.savetxt(outfile_embedding, embedding, delimiter='\t', fmt='%.2f')
for s in ("lambdas","vectors"):
    out = outfile_result % (s)
    np.savetxt(out, result[s], delimiter='\t', fmt='%.2f')
