# SanicView

The FITS viewer for when you need to go fast (over SSH)

## But.. Why?

Sometimes you just want to take a quick look at a FITS image on a remote server, and you don't want to wait a minute or more for ds9 to appear. Then after the window appears, you probably want some level of interactivity with minimal latency. That is the experience SanicView aims to provide, possibly at the cost of resource usage.

## FITS image?

Astronomy data is typically stored in FITS (Flexible Image Transport System) files, an old and uncompressed format intended to store metadata, images, and tables. These files are often annoying large; a 4096x4096 image at the standard 16 bits/pixel can be 33 MB and doesn't compress particularly well (usually by <50%). When doing telescope or camera diagnostics it's common to require remote access to this data, sometimes over great distance and over low-bandwidth and high-latency connections.


# Internal Interface

`Window` and `Event` encapsulate xcb, an implementation of X window system, and together with `PixelTable`, `Widget`, and `Image` comprise a very small GUI library that can be extended to build applications or re-implemented to use a different window system (Mir?).

## Drawing in SanicView

Drawing is only done through `Window::draw_point()`, which stores a desired change to a single pixel on the window. The window collects all the changes that widgets have requested and applies them to the window. At an interval no faster than the supplied framerate (default 60 frames/second) `Window` will collect all changes from its widgets and draw them on the screen.

`Window` will ensure that only pixels that were actually changed are redrawn, but at the cost of CPU usage. If they can do so at zero cost, widgets should avoid requesting changes that do not modify the current display.

Widgets are limited to drawing within the rectangular region defined by their position and dimensions. Only the most recent request to change a pixel will be honored, and younger widgets will overwrite changes by older widgets.

## Events in SanicView

When an event of any kind is produced, an `Event` is passed to every widget's `handle()` method in the order the widgets were constructed. If a widget returns `true` from its `handle()`, the event will not be passed to any other widgets.

## Command line options

```
sanicview filename [-w width] [-h height] [-f framerate]
```