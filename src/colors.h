#pragma once

struct color {
    uint value;

    color(unsigned char r, unsigned char g, unsigned char b) {
        uint res = (r << 16) | (g << 8) | b;
        value = res;
    }

    color(uint v): value(v) { }

    unsigned char r() const {
        return (value >> 16) & 255;
    }

    void r(unsigned char new_v) {
        value = value & ((255 << 8) | 255) | (new_v << 16);
    }

    unsigned char  g() const {
        return (value >> 8) & 255;
    }

    void g(unsigned char new_v) {
        value = value & ((255 << 16) | 255) | (new_v << 8);
    }

    unsigned char b() const {
        return value & 255;
    }

    void b(unsigned char new_v) {
        value = value & ((255 << 16) | (255 << 8)) | new_v;
    }
};

color red(255 << 16);
color green(255 << 8);
color blue(255);
color white(255, 255, 255);
