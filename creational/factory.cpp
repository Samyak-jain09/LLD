#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Product Interface
class Document {
public:
    virtual string getHeader() const = 0;
    virtual string formatRow(const vector<string>& data) const = 0;
    virtual string getFooter() const = 0;
    virtual string getFileExtension() const = 0;

    virtual ~Document() = default;
};

// Concrete Products
class PdfDocument : public Document {
public:
    string getHeader() const override {
        return "PDF DOCUMENT";
    }

    string formatRow(const vector<string>& data) const override {
        string result = "| ";
        for (size_t i = 0; i < data.size(); i++) {
            result += data[i];
            if (i != data.size() - 1)
                result += " | ";
        }
        result += " |";
        return result;
    }

    string getFooter() const override {
        return "--- PDF DOCUMENT END ---";
    }

    string getFileExtension() const override {
        return ".pdf";
    }
};

class HtmlDocument : public Document {
public:
    string getHeader() const override {
        return "<html><body><table>";
    }

    string formatRow(const vector<string>& data) const override {
        string result = "<tr>";
        for (const auto& cell : data) {
            result += "<td>" + cell + "</td>";
        }
        result += "</tr>";
        return result;
    }

    string getFooter() const override {
        return "</table></body></html>";
    }

    string getFileExtension() const override {
        return ".html";
    }
};

class CsvDocument : public Document {
public:
    string getHeader() const override {
        return "";
    }

    string formatRow(const vector<string>& data) const override {
        string result;
        for (size_t i = 0; i < data.size(); i++) {
            result += data[i];
            if (i != data.size() - 1)
                result += ",";
        }
        return result;
    }

    string getFooter() const override {
        return "";
    }

    string getFileExtension() const override {
        return ".csv";
    }
};

// Creator
class AbstractExportCreator {
public:
    virtual unique_ptr<Document> createDocument() const = 0;

    void exportData(const vector<vector<string>>& data,
                    ostream& out = cout) const {

        auto doc = createDocument();

        out << "Exporting to " << doc->getFileExtension()
            << " format...\n";

        if (auto header = doc->getHeader(); !header.empty())
            out << header << '\n';

        for (const auto& row : data)
            out << doc->formatRow(row) << '\n';

        if (auto footer = doc->getFooter(); !footer.empty())
            out << footer << '\n';

        out << "Export complete.\n\n";
    }

    virtual ~AbstractExportCreator() = default;
};

// Concrete Creators
class PdfExportCreator : public AbstractExportCreator {
public:
    unique_ptr<Document> createDocument() const override {
        return make_unique<PdfDocument>();
    }
};

class HtmlExportCreator : public AbstractExportCreator {
public:
    unique_ptr<Document> createDocument() const override {
        return make_unique<HtmlDocument>();
    }
};

class CsvExportCreator : public AbstractExportCreator {
public:
    unique_ptr<Document> createDocument() const override {
        return make_unique<CsvDocument>();
    }
};

int main() {
    vector<vector<string>> reportData = {
        {"Name", "Department", "Salary"},
        {"Alice", "Engineering", "120000"},
        {"Bob", "Marketing", "95000"},
        {"Charlie", "Design", "105000"}
    };

    PdfExportCreator pdfExporter;
    HtmlExportCreator htmlExporter;
    CsvExportCreator csvExporter;

    pdfExporter.exportData(reportData);
    htmlExporter.exportData(reportData);
    csvExporter.exportData(reportData);

    return 0;
}