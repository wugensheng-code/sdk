#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>

#define STBI_NO_STDIO

#ifdef __cplusplus
extern "C" {
#endif

// #ifdef OPENCV
// void *open_video_stream(const char *f, int c, int w, int h, int fps);
// image get_image_from_stream(void *p);
// image load_image_cv(char *filename, int channels);
// int show_image_cv(image im, const char* name, int ms);
// #endif

typedef enum{
    PNG, BMP, TGA, JPG
} IMTYPE;

typedef struct {
    int w;
    int h;
    float scale;
    float rad;
    float dx;
    float dy;
    float aspect;
} augment_args;

typedef struct {
    int w;
    int h;
    int c;
    float *data;
} image;

typedef struct{
    float x, y, w, h;
} box;

typedef struct detection{
    box bbox;
    int classes;
    float *prob;
    float *mask;
    float objectness;
    int sort_class;
} detection;

float get_color(int c, int x, int max);
image mask_to_rgb(image mask);
static float get_pixel(image m, int x, int y, int c);
static float get_pixel_extend(image m, int x, int y, int c);
static void set_pixel(image m, int x, int y, int c, float val);
static void add_pixel(image m, int x, int y, int c, float val);
static float bilinear_interpolate(image im, float x, float y, int c);
void composite_image(image source, image dest, int dx, int dy);
image border_image(image a, int border);
image tile_images(image a, image b, int dx);
image get_label(image **characters, char *string, int size);
void draw_label(image a, int r, int c, image label, const float *rgb);
void draw_box(image a, int x1, int y1, int x2, int y2, float r, float g, float b);
void draw_box_width(image a, int x1, int y1, int x2, int y2, int w, float r, float g, float b);
void draw_bbox(image a, box bbox, int w, float r, float g, float b);
image **load_alphabet();
void draw_detections(image im, detection *dets, int num, float thresh, char **names, image **alphabet, int classes);
void transpose_image(image im);
void rotate_image_cw(image im, int times);
void flip_image(image a);
image image_distance(image a, image b);
void ghost_image(image source, image dest, int dx, int dy);
void blocky_image(image im, int s);
void censor_image(image im, int dx, int dy, int w, int h);
void embed_image(image source, image dest, int dx, int dy);
image collapse_image_layers(image source, int border);
void constrain_image(image im);
void normalize_image(image p);
void normalize_image2(image p);
void copy_image_into(image src, image dest);
image copy_image(image p);
void rgbgr_image(image im);
int show_image(image p, const char *name, int ms);
void save_image_options(image im, const char *name, IMTYPE f, int quality);
void save_image(image im, const char *name);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);
image make_empty_image(int w, int h, int c);
image make_image(int w, int h, int c);
image make_random_image(int w, int h, int c);
image float_to_image(int w, int h, int c, float *data);
void place_image(image im, int w, int h, int dx, int dy, image canvas);
image center_crop_image(image im, int w, int h);
image rotate_crop_image(image im, float rad, float s, int w, int h, float dx, float dy, float aspect);
image rotate_image(image im, float rad);
void fill_image(image m, float s);
void translate_image(image m, float s);
void scale_image(image m, float s);
image crop_image(image im, int dx, int dy, int w, int h);
int best_3d_shift_r(image a, image b, int min, int max);
int best_3d_shift(image a, image b, int min, int max);
void composite_3d(char *f1, char *f2, char *out, int delta);
void letterbox_image_into(image im, int w, int h, image boxed);
image letterbox_image(image im, int w, int h);
image resize_max(image im, int max);
image resize_min(image im, int min);
image random_crop_image(image im, int w, int h);
augment_args random_augment_args(image im, float angle, float aspect, int low, int high, int w, int h);
image random_augment_image(image im, float angle, float aspect, int low, int high, int w, int h);
float three_way_max(float a, float b, float c);
float three_way_min(float a, float b, float c);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void grayscale_image_3c(image im);
image grayscale_image(image im);
image threshold_image(image im, float thresh);
image blend_image(image fore, image back, float alpha);
void scale_image_channel(image im, int c, float v);
void translate_image_channel(image im, int c, float v);
image binarize_image(image im);
void saturate_image(image im, float sat);
void hue_image(image im, float hue);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void random_distort_image(image im, float hue, float saturation, float exposure);
void saturate_exposure_image(image im, float sat, float exposure);
image resize_image(image im, int w, int h);
void test_resize(char *filename);
image load_image_stb(char *filename, int channels);
image load_image(char *filename, int w, int h, int c);
image load_image_color(char *filename, int w, int h);
image get_image_layer(image m, int l);
void print_image(image m);
image collapse_images_vert(image *ims, int n);
image collapse_images_horz(image *ims, int n);
void show_image_normalized(image im, const char *name);
void show_images(image *ims, int n, char *window);
void free_image(image m);

void draw_function(image im, int x0, int y0, int (*f)(int), int start, int end, float r, float g, float b);

image load_image_from_memory(char *buffer, int len, int w, int h, int c);
image load_image_stb_from_memory(char *buffer, int len, int channels);

#ifdef __cplusplus
}
#endif

#endif

