#include <iostream>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkExtractImageFilter.h>

int main(int argc, char *argv[])
{
    // Check for the correct number of command line arguments
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " inputImage outputPrefix sliceDimension" << std::endl;
        return EXIT_FAILURE;
    }

    const char *inputImageFileName = argv[1];
    const char *outputPrefix = argv[2];
    const unsigned int sliceDimension = std::atoi(argv[3]);

    // Define input image types
    using PixelType = unsigned char;
    constexpr unsigned int InputDimension = 3;
    constexpr unsigned int OutputDimension = 2;
    using InputImageType = itk::Image<PixelType, InputDimension>;
    using OutputImageType = itk::Image<PixelType, OutputDimension>;

    // Load the 3D input image
    using ReaderType = itk::ImageFileReader<InputImageType>;
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

    InputImageType::Pointer inputImage = reader->GetOutput();

    // Create an image iterator for the input image
    itk::ImageRegionIterator<InputImageType> imageIterator(inputImage, inputImage->GetLargestPossibleRegion());

    // Define an extractor filter
    using ExtractorType = itk::ExtractImageFilter<InputImageType, OutputImageType>;
    ExtractorType::Pointer extractor = ExtractorType::New();
    extractor->SetDirectionCollapseToSubmatrix();

    // Loop through the slices
    for (unsigned int sliceNumber = 0; sliceNumber < inputImage->GetLargestPossibleRegion().GetSize()[sliceDimension]; ++sliceNumber)
    {
        // Extract a slice
        extractor->SetInput(inputImage);
        itk::ImageRegion<InputDimension> sliceRegion = inputImage->GetLargestPossibleRegion();
        sliceRegion.SetSize(sliceDimension, 0);
        sliceRegion.SetIndex(sliceDimension, sliceNumber);
        extractor->SetExtractionRegion(sliceRegion);
        extractor->Update();

        // Save the 2D image
        std::string outputFileName = outputPrefix + std::to_string(sliceNumber) + ".png";
        using WriterType = itk::ImageFileWriter<OutputImageType>;
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
