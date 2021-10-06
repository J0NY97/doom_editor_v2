# libui_v3
3rd time's the charm.

Only works with SDL

![alt text](https://github.com/J0NY97/libui_v3/blob/master/images/demo.PNG?raw=true)

## UI File
### HOW TO
```c
t_ui_layout   layout;
SDL_Event     e;

ui_load(&layout, "file.ui");
while (SDL_PollEvent(&e))
{
    ui_layout_event(&layout, e);
}
ui_layout_render(&layout);
```
### Syntax
```
[element type] [variable name]
{
    [arg] : [value];
}
```
example :
```
Button button_name
{
    pos : 10, 10, 50, 20;
}
```
Valid element types:
`Window`,
`Element`,
`Label`,
`Button`,
`Menu`,
`Input`,
`Slider`,
`Checkbox`  
(Everything else will be ignored)  
Valid args:
`pos`,
`bg_color`,
`bg_image`,
`title`,
`font_path`,
`font_size`,
`font_color`,
`text_align`,
`value`  
(All args that are not one of the above, will be treated as a child element)

Child example :
```
Label label0
{
    title : "button";
    text_align : center left;
}

Button button0
{
    pos : 10, 10, 50, 20;
    label0;
}
```
### Element
Every element type is an element.  
If you give any element type an element as child, it will edit the base values of that element.  
`pos`, `bg_color`, `bg_image`, `...`  
### Label
Text element.  
Can't take any type of element as child.  
Can give to any type of element with a label. (button, dropdown, slider, ...)  
`pos`, `title`, `font_path`, `font_size`, `font_color`, `text_align`, `...`  
### Button
Can take in `Label`, `Element`.  
`pos`, `bg_color`, `bg_image`, `title`, `font_path`, `font_size`, `font_color`, `text_align`, `...`  
### Menu
Can take in any other type of element (it will add it as child, except ´Element´).  
`pos`, `bg_color`, `bg_image`, `...`
### Input
Text Input.  
Can take in `Label`, `Element`.  
`pos`, `bg_color`, `bg_image`, `...`  
### Slider
Can take in `Element`, `Label`, `Button`.  
`pos`, `bg_color`, `bg_image`, `value`, `...`  
### Checkbox
Can't take in anything.  
`pos`, `bg_color`, `bg_image`, `...`  

### Other Important Stuff
```
pos : 10, 10, 50, 20;
pos.x : 10;
pos.xy : 10, 10;
pos.xyw : 10, 10, 50;
pos.xywh : 10, 10, 50, 20;
pos : 0.1, 0.1, 0.5, 0.5; // translates to percentage of it's parent's w/h
```
```
bg_color : [default], [hover], [click];
bg_color : 0xffff0000;
bg_color : 0xffff0000, 0xffff0000;
bg_color : 0xffff0000, 0xffff0000, 0xffff0000;
```
```
bg_image : [default], [hover], [click];
bg_image : "image.png";
bg_image : "image.png", "image_hover.png";
bg_image : "image.png", "image_hover.png", image_click.png";
```
```
title : "This is the title of the label";
font_path : "direct_path_to_the_font.ttf";
font_size : 12;
font_color : 0xffff0000;
```
```
text_align : [center, left, right, bot, up, none];
text_align : center;
text_align : center left;
text_align : center left bot;
```
```
value : [value], [min], [max];
value : 125, 0, 255;
```

## TODO
- [ ] readme
  - [ ] How to create elements in code.
### Elements
- [x] window
- [x] label
- [x] button
- [x] menu
- [x] input
- [x] slider
- [x] checkbox
- [ ] tab
- [ ] radio

###IGNORE
```
ui_element_set_pos(t_ui_element *, t_vec4);
```
NOTE: if you intend on using relative position (0.1 aka 10% of parent.w/h), it will take the relativity from its current parent, so if you change the parent after this it still is taking the pos from the last parent, which means you have call this again.

