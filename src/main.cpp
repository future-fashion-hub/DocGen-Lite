#include <exception>
#include <string>

#include "Config.h"
#include "FileScanner.h"
#include "FileUtils.h"
#include "HTMLGenerator.h"
#include "Logger.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
    Logger logger(Logger::LogLevel::Info);

    Config config;
    if (!config.loadFromArgs(argc, argv)) {
        logger.error("Usage: docgen -i <input_dir> -o <output_dir> [--exclude dir1,dir2] [--open]");
        return 1;
    }

    std::string validationError;
    if (!config.validate(&validationError)) {
        logger.error(validationError);
        return 1;
    }

    try {
        FileScanner scanner(config, logger);
        std::vector<SourceFile> files = scanner.scanDirectory();

        Parser parser;
        parser.addObserver(&logger);
        std::size_t classCount = 0;
        std::size_t methodCount = 0;

        for (auto& file : files) {
            parser.parseFile(file);
            for (const auto& entity : file.getEntities()) {
                if (entity->kind() == "class") {
                    ++classCount;
                }
                if (entity->kind() == "method") {
                    ++methodCount;
                }
            }
        }

        HTMLGenerator generator(config.outputPath(), logger);
        generator.generate(files);
        parser.notifyGenerationFinished();

        logger.info("Generation report:");
        logger.info("Processed files: " + std::to_string(files.size()));
        logger.info("Found classes: " + std::to_string(classCount));
        logger.info("Found functions: " + std::to_string(methodCount));

        if (config.shouldOpenAfterGeneration()) {
            const std::string indexPath = fileutils::joinPath(config.outputPath(), "index.html");
            if (fileutils::openInBrowser(indexPath)) {
                logger.info("Opened documentation: " + indexPath);
            } else {
                logger.warning("Could not open documentation automatically: " + indexPath);
            }
        }

        logger.info("Done.");
    } catch (const std::exception& ex) {
        logger.error(std::string("Fatal error: ") + ex.what());
        return 2;
    }

    return 0;
}
