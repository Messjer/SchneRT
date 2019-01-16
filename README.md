SCHNEll Ray Tracer
------------------
Ray Tracer project for THU Course `Advanced Computer Graphics`

## Features
- Monte Carlo Path Tracing
    - Specular Reflection
    - Diffuse Reflection
    - Refraction
- Supported objects:
    - Spheres
    - Planes
    - Axis-aligned boxes
    - Bezier rotational surface (*WARNING* can be devastatingly slow)
        - intersection solved using Newton's method, attempt = 10, iteration = 5
- Natively supports smooth shadows and caustics by MCPT.
- Cornell Box Scene modified from `Smallpt`
- Scene specification referenced from Raina Tam's implementation (private conversation).

## References
- [Smallpt](http://www.kevinbeason.com/smallpt/)
- [Scratch Pixel](https://www.scratchapixel.com/)
- Student Raina Tam's implementation (PPM & Bezier curve)