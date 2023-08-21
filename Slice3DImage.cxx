#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkExtractImageFilter.h>

int main(int argc, char *argv[])
{
    // Check for the correct number of command line arguments
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " inputImage outputPrefix" << std::endl;
        return EXIT_FAILURE;
    }

    const char *inputImageFileName = argv[1];
    const char *outputPrefix = argv[2];
    const unsigned int sliceDimension = 2;

    // Define input image types
    using PixelType = unsigned char;
    constexpr unsigned int Dimension = 3;
    using ImageType = itk::Image<PixelType, Dimension>;

    // Load the 3D input image
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputImageFileName);
    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
        std::cerr << "Error reading the input image: " << e << std::endl;
        return EXIT_FAILURE;
    }

    ImageType::Pointer inputImage = reader->GetOutput();

    // Create an image iterator for the input image
    itk::ImageRegionIterator<ImageType> imageIterator(inputImage, inputImage->GetLargestPossibleRegion());

    // Define an extractor filter
    using ExtractorType = itk::ExtractImageFilter<ImageType, ImageType>;
    ExtractorType::Pointer extractor = ExtractorType::New();

    // Loop through the slices
    for (unsigned int sliceNumber = 0; sliceNumber < inputImage->GetLargestPossibleRegion().GetSize()[sliceDimension]; ++sliceNumber)
    {
        // Extract a slice
        extractor->SetInput(inputImage);
        itk::ImageRegion<Dimension> sliceRegion = inputImage->GetLargestPossibleRegion();
        sliceRegion.SetSize(sliceDimension, 0);
        sliceRegion.SetIndex(sliceDimension, sliceNumber);
        extractor->SetExtractionRegion(sliceRegion);
        extractor->Update();

        // Save the 2D image
        std::string outputFileName = outputPrefix + std::to_string(sliceNumber) + ".png";
        using WriterType = itk::ImageFileWriter<ImageType>;
        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName(outputFileName);
        writer->SetInput(extractor->GetOutput());

        try
        {
            writer->Update();
        }
        catch (itk::ExceptionObject &e)
        {
            std::cerr << "Error writing the output image: " << e << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
