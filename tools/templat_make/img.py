import os
import cv2 as cv
import numpy as np
import argparse


def erosion(src):
    erosion_size = 2
    erosion_type = 0
    val_type = 0
    if val_type == 0:
        erosion_type = cv.MORPH_RECT
    elif val_type == 1:
        erosion_type = cv.MORPH_CROSS
    elif val_type == 2:
        erosion_type = cv.MORPH_ELLIPSE

    element = cv.getStructuringElement(erosion_type, (2*erosion_size + 1, 2*erosion_size+1), (erosion_size, erosion_size))
    erosion_dst = cv.erode(src, element)
    return erosion_dst


def dilatation(src):
    dilatation_size = 1
    dilatation_type = 0
    val_type = 0
    if val_type == 0:
        dilatation_type = cv.MORPH_RECT
    elif val_type == 1:
        dilatation_type = cv.MORPH_CROSS
    elif val_type == 2:
        dilatation_type = cv.MORPH_ELLIPSE
    element = cv.getStructuringElement(dilatation_type, (2*dilatation_size + 1, 2*dilatation_size+1), (dilatation_size, dilatation_size))
    dilatation_dst = cv.dilate(src, element)
    return dilatation_dst


def binary_img(src_mat):
    dil_mat = dilatation(src_mat)
    ero_mat = erosion(dil_mat)
    gray_mat = cv.cvtColor(ero_mat, cv.COLOR_BGR2GRAY)
    binary = cv.adaptiveThreshold(gray_mat, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 25, 10)
    return binary


def fmt_dir(dir, out):
    if not (os.path.exists(out) and os.path.isdir(out)):
        try:
            os.mkdir(out)
        except:
            pass

    for img in os.listdir(dir):
        img_suffix = img.find("jpg")
        if img_suffix == -1:
            continue
        img_name = img.split("-")[0]

        src_img = os.path.join(dir, img)
        src_mat = cv.imread(src_img)
        bin_mat = binary_img(src_mat)
        dst_img = os.path.join(out, "%s.jpg", img_name)

        row = bin_mat.shape[0]
        col = bin_mat.shape[1]

        v_w = 5
        h_w = 10
        bin_mat[0:v_w, ] = 255
        bin_mat[row-v_w:, ] = 255
        bin_mat[:, 0:h_w] = 255
        bin_mat[:, col-h_w:] = 255

        cv.imwrite(dst_img, bin_mat)
        print("img ", bin_mat.shape)


def draw_contours(contours, mat):
    for contour in contours:
        area = cv.contourArea(contour)
        x, y, w, h = cv.boundingRect(contour)
        print(w, h, area)
        cv.rectangle(mat, (x, y), (x+w, y+h), (153, 0, 0), 2)

    return mat


def valid_contour(contours):
    c = []
    for contour in contours:
        area = cv.contourArea(contour)
        if area < 100:
            continue
        c.append(contour)

    return c



def generate_template(src_img_dir, templat_out_dir):
    if not (os.path.exists(templat_out_dir) and os.path.isdir(templat_out_dir)):
        try:
            os.mkdir(templat_out_dir)
        except:
            pass

    for img in os.listdir(src_img_dir):
        img_suffix = img.find("jpg")
        if img_suffix == -1:
            continue

        img_name = img.split("-")[0]
        one = img_name[0]
        two = img_name[1]

        src_img = os.path.join(src_img_dir, img)
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

        contours, hierarchy = cv.findContours(bin_out_mat, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_NONE)
        contours = valid_contour(contours)
        if len(contours) != 2:
            print("img contours error: ", img, len(contours))
            o_m = draw_contours(contours, bin_out_mat)
            cv.imshow("exception mat", o_m)
            cv.imshow("src", src_mat)
            cv.waitKey()
            continue

        x1, y1, w1, h1 = cv.boundingRect(contours[0])
        x2, y2, w2, h2 = cv.boundingRect(contours[1])

        one_mat = np.zeros((40, 40), np.uint8)
        two_mat = np.zeros((40, 40), np.uint8)

        if x1 < x2:
            one_mat[0:h1, 0:w1] = bin_out_mat[y1:y1+h1, x1:x1+w1]
            two_mat[0:h2, 0:w2] = bin_out_mat[y2:y2+h2, x2:x2+w2]
        else:
            one_mat[0:h2, 0:w2] = bin_out_mat[y2:y2+h2, x2:x2+w2]
            two_mat[0:h1, 0:w1] = bin_out_mat[y1:y1+h1, x1:x1+w1]

        one_out_file = os.path.join(templat_out_dir, "%s.jpg" % one)
        two_out_file = os.path.join(templat_out_dir, "%s.jpg" % two)
        cv.imwrite(one_out_file, one_mat)
        cv.imwrite(two_out_file, two_mat)

