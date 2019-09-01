from uwimg import *
im = load_image("data/dog.jpg")

f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

thumb = nn_resize(blur, blur.w//7, blur.h//7)
save_image(thumb, "dogthumb")

f = make_emboss_filter()
emb = convolve_image(im, f, 1)
clamp_image(emb)
save_image(emb, "dog_emboss")

f = make_highpass_filter()
save_image(convolve_image(im, f, 0), "dog_highpass")

f = make_sharpen_filter()
sharp = convolve_image(im, f, 1)
clamp_image(sharp)
save_image(sharp, "dog_sharpen")

f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")