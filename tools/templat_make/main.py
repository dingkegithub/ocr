import img
import sys

if __name__ == '__main__':
    src_img_dir = sys.argv[1]
    templat_out_dir = sys.argv[2]
    img.generate_template(src_img_dir, templat_out_dir)
