// conversionservice.cpp
// Implements ConversionService, a high-level interface for conversion operations.
#include "conversionservice.h"

ConversionService::ConversionService(QObject *parent)
    : QObject(parent), converter(new Converter(this)), converting(false)
{
    connect(converter, &Converter::conversionProgress, this, &ConversionService::conversionProgress);
    connect(converter, &Converter::conversionStarted, this, &ConversionService::conversionStarted);
    connect(converter, &Converter::conversionFinished, this, [this](bool success, const QString &msg) {
        converting = false;
        emit conversionFinished(success, msg);
    });
}

ConversionService::~ConversionService() {}

void ConversionService::startConversion(const ConversionSettings &settings)
{
    if (converting) return;
    converting = true;
    emit conversionStarted();
    converter->startConversion(settings);
}

void ConversionService::cancelConversion()
{
    if (converting) {
        converter->cancelConversion();
        converting = false;
    }
}

bool ConversionService::isConverting() const
{
    return converting;
}
