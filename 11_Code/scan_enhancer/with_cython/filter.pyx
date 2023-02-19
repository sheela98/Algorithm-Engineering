### Cython compiler directives ###
# cython: language_level = 3
# cython: boundscheck = False
# cython: wraparound = False
# cython: cdivision = True
##################################


# zwei numpy arrays
def custom_filter(double[:,:] blurred_img, double[:,:] gray_img):
    cdef:
        int i, j
        double d, m, gamma
    for i in range(gray_img.shape[0]):
        for j in range(gray_img.shape[1]):
            d = gray_img[i][j] / (blurred_img[i][j] + 0.00000000001)
            m = min(255, d * 255 + 0.00000000001)
            gamma = 0.5 * (m / 255.0) * m * m / 65025
            m = ((m / 255.0) ** (1 / (gamma + 0.00000000001))) * 255.0
            gray_img[i][j] = m
