import img
import cv2 as cv
import numpy as np


class DigitalOcr:
    def __init__(self, template_path):
        self.path = template_path
        self.template = {}
        self.v_w = 5
        self.h_w = 10
        self.load_template()

    def load_template(self):
        for template in os.listdir(self.path):
            if template.find("jpg") < 0:
                continue

            name = template.split(".")[0]
            name_mat = cv.imread(os.path.join(self.path, template), cv.COLOR_BGR2GRAY)
            self.template[name] = name_mat

    def recognise(self, img):
        img_mat = cv.imread(img)
        bin_mat = binary_img(img_mat)
        row = bin_mat.shape[0]
        col = bin_mat.shape[1]
        bin_mat[0:self.v_w, ] = 255
        bin_mat[row-self.v_w:, ] = 255
        bin_mat[:, 0:self.h_w] = 255
        bin_mat[:, col-self.h_w:] = 255

        min_match = [None, None]
        for k, v in self.template.items():
            w = bin_mat.shape[1] - v.shape[1] + 1
            h = bin_mat.shape[0] - v.shape[0] + 1
            result = np.zeros((h, w), np.float)
            #result = cv.matchTemplate(bin_mat, v, cv.TM_SQDIFF_NORMED, result)
            result = cv.matchTemplate(bin_mat, v, cv.TM_SQDIFF_NORMED)
            minVal, maxVal, minLoc, maxLoc = cv.minMaxLoc(result)
            if min_match[0] is None or minVal < min_match[0]:
                min_match[0] = minVal
                min_match[1] = k
        return min_match

    def recognise2(self, src_img):
        src_mat = cv.imread(src_img)
        bin_mat = binary_img(src_mat)

        row = bin_mat.shape[0]
        col = bin_mat.shape[1]

        v_w = 5
        h_w = 10
        bin_mat[0:v_w, ] = 255
        bin_mat[row-v_w:, ] = 255
        bin_mat[:, 0:h_w] = 255
        bin_mat[:, col-h_w:] = 255

        bin_out_mat = np.zeros(bin_mat.shape, np.uint8)
        for i in range(bin_mat.shape[1]):
            for j in range(bin_mat.shape[0]):
               if bin_mat[j][i] > 200:
                   bin_out_mat[j][i] = 0
               else:
                   bin_out_mat[j][i] = 255

        # cv.imshow("bin-mat", bin_out_mat)

        contours, hierarchy = cv.findContours(bin_out_mat, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
        contours = valid_contour(contours)
        if len(contours) != 2:
            print("img contours error: ", src_mat, len(contours))
            o_m = draw_contours(contours, bin_out_mat)
            cv.imshow("exception mat", o_m)
            cv.imshow("src", src_mat)
            cv.waitKey()
            return None

        x1, y1, w1, h1 = cv.boundingRect(contours[0])
        x2, y2, w2, h2 = cv.boundingRect(contours[1])

        one_mat = np.zeros((50, 50), np.uint8)
        two_mat = np.zeros((50, 50), np.uint8)

        if x1 < x2:
            one_mat[0:h1, 0:w1] = bin_out_mat[y1:y1+h1, x1:x1+w1]
            two_mat[0:h2, 0:w2] = bin_out_mat[y2:y2+h2, x2:x2+w2]
        else:
            one_mat[0:h2, 0:w2] = bin_out_mat[y2:y2+h2, x2:x2+w2]
            two_mat[0:h1, 0:w1] = bin_out_mat[y1:y1+h1, x1:x1+w1]

        mats = [one_mat, two_mat]
        min_match = [None, None, None, None]

        for idx, m in enumerate(mats):
            for k, v in self.template.items():
                # print(m.shape, v.shape)
                result = cv.matchTemplate(m, v, cv.TM_SQDIFF_NORMED)
                minVal, maxVal, minLoc, maxLoc = cv.minMaxLoc(result)

                if min_match[0 + idx*2] is None or minVal < min_match[0 + idx*2]:
                    min_match[0 + idx*2] = minVal
                    min_match[1 + idx*2] = k

        return min_match


def test_recognise():
    ocr = DigitalOcr("/Users/dkos/Downloads/split")

    val_imags = "/users/dkos/Downloads/img"

    for img in os.listdir(val_imags):
        if img.find("jpg") < 0:
            continue
        res = ocr.recognise(os.path.join(val_imags, img))
        img_num = int(img.split('-')[0])
        if img_num != int(res[1]):
            print(img, " want: ", img_num, " actual: ", res[1])


def test_recognise2():
    ocr = DigitalOcr("/Users/dkos/Downloads/split")

    val_imags = "/users/dkos/Downloads/img"

    for img in os.listdir(val_imags):
        if img.find("jpg") < 0:
            continue

        res = ocr.recognise2(os.path.join(val_imags, img))
        # print("result: ", res)

        actual_num = int(res[1]) * 10 + int(res[3])

        img_num = int(img.split('-')[0])

        if img_num != actual_num:
            print(img, " want: ", img_num, " actual: ", actual_num)


#test_recognise2()

def generate_lib(p):
    out_file = open("/Users/dkos/Downloads/ocr.model", "wb")

    for img in os.listdir(p):
        if img.find(".jpg") < 0:
            continue

        name = img.split(".")[0]

        with open(os.path.join(p, img), mode="rb") as fd:
            content = fd.read()
            out_file.write(str.encode("name-"))
            out_file.write(str.encode(name))
            out_file.write(str.encode("-img-"))
            out_file.write(content)
            out_file.write(str.encode("-dkos-ocr-sokd-"))
            #out_file.write(f"name-{str.encode(name)}-img-{content}-dkos-ocr-sokd-")


#generate_lib("/Users/dkos/Downloads/split")


def parse_lib(model_file):
    out_dir = "/Users/dkos/Downloads/ak"
    try:
        os.mkdir(out_dir)
    except:
        pass

    with open(model_file, "rb") as rfd:
        content = rfd.read()
        cs = content.split(str.encode("-dkos-ocr-sokd-"))
        for c in cs:
            if c[:4] != b"name":
                print(c[:4])
                continue

            idx = c.find(str.encode("-img-"))
            img_name = c[5:idx]
            img_content = c[idx+5:]

            with open(os.path.join(out_dir, f"{bytes.decode(img_name)}.jpg"), 'wb') as wfd:
                wfd.write(img_content)


parse_lib("/Users/dkos/Downloads/ocr.model")

