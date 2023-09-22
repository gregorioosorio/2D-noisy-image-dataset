import cv2
import numpy as np
import matplotlib.pyplot as plt
from skimage.metrics import structural_similarity as ssim

# Load the generated images and the reference image (ground truth)
adf_path = '../img/adf/noisy_3_127_adf_200_0.005_1.png'
cnn_path = '../img/dataset/clean/clean_127.png'
ref_path = '../img/dataset/clean/clean_127.png'

image_adf = cv2.imread(adf_path, cv2.IMREAD_GRAYSCALE)
image_cnn = cv2.imread(cnn_path, cv2.IMREAD_GRAYSCALE)
image_ref = cv2.imread(ref_path, cv2.IMREAD_GRAYSCALE)

# Calculate MSE for both images compared to the reference image
mse_adf = np.mean((image_ref - image_adf) ** 2)
mse_cnn = np.mean((image_ref - image_cnn) ** 2)

# Calculate PSNR for both images compared to the reference image
max_pixel_value = 255  # Assuming the images are 8-bit grayscale
psnr_adf = 20 * np.log10(max_pixel_value / np.sqrt(mse_adf))
psnr_cnn = 20 * np.log10(max_pixel_value / np.sqrt(mse_cnn))

# Calculate SSIM for both images compared to the reference image
ssim_adf = ssim(image_ref, image_adf)
ssim_cnn = ssim(image_adf, image_cnn)

# Plot the results
metrics = ['MSE', 'PSNR (dB)', 'SSIM']
adf_values = [mse_adf, psnr_adf, ssim_adf]
cnn_values = [mse_cnn, psnr_cnn, ssim_cnn]

x = np.arange(len(metrics))
width = 0.35

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, adf_values, width, label='Anisotropic Filter')
rects2 = ax.bar(x + width/2, cnn_values, width, label='CNN - U-Net')

ax.set_ylabel('Metric Value')
ax.set_title('Comparison of Metrics Between Methods')
ax.set_xticks(x)
ax.set_xticklabels(metrics)
ax.legend()

plt.show()

# Conclusion
print("Conclusion:")
if mse_adf < mse_cnn:
    print("The Anisotropic Filter method is more efficient in terms of MSE.")
else:
    print("The CNN method is more efficient in terms of MSE.")

if psnr_adf > psnr_cnn:
    print("The Anisotropic Filter method is more efficient in terms of PSNR.")
else:
    print("The CNN method is more efficient in terms of PSNR.")

if ssim_adf > ssim_cnn:
    print("The Anisotropic Filter method is more efficient in terms of SSIM.")
else:
    print("The CNN method is more efficient in terms of SSIM.")