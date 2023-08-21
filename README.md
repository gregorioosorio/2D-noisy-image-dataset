# 2D-noisy-image-dataset

The goal of this repository is to programatically create a dataset of noisy 2D images from a 3D MRI.

## Source RMI image

Two RMI images were downloaded from [brainweb project](https://brainweb.bic.mni.mcgill.ca/selection_normal.html)
- Clean RMI
  - *Parameters*: Modality=T1, Protocol=ICBM, Phantom_name=normal, Slice_thickness=1mm, Noise=0%, INU=20%
  - *Path*: img/mnc/t1_icbm_normal_1mm_pn0_rf20.mnc.gz
- Noisy RMI
  - *Parameters*: Modality=T1, Protocol=ICBM, Phantom_name=normal, Slice_thickness=1mm, Noise=9%, INU=20%
  - *Path*: img/mnc/t1_icbm_normal_1mm_pn9_rf20.mnc.gz

## Convert mnc to nii image

In order to use the ITK we need the nii RMI format.

*Note 1*: Before running this command you must install the mnc2nii toolkit: https://bic-mni.github.io/#using.

*Note 2*: this script compress the nii image using gzip.

In order to convert the mnc image to nii run this command: 
```bash
$ ./convert_images.bsh
```

## Slice 3D image

Using ITK slice the 3D image in a set of 2D images

```bash
$ ./build/Slice3DImage input3DImage outputPrefix
```