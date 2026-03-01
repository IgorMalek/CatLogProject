🐾 CatLog
CatLog is a modern C++ console application designed to help pet owners track and manage the daily life, health, and milestones of their cats.

Built with modern C++ (including C++20 Ranges) and Object-Oriented Programming principles, it features persistent local storage, automated statistics, and even an integration to view cat photos via an external API.

✨ Features
🐈 Cat Registry: Register multiple cats with specific details such as breed, date of birth, and fur color.

📝 Event Logging: Keep a precise history of your cats' lives. Log feeding amounts (in grams), weight updates (in kg), and medicine administrations with automatic timestamps.

📊 Smart Reports: Generate comprehensive health reports showing total food consumed, average meal sizes, weight fluctuations, and a quick view of the 5 most recent events.

🕰️ Age Converter: Calculate your cats' exact age and convert it into "human years" using a highly optimized C++20 <ranges> processing pipeline.

🖼️ API Integration: View photos of cats matching your cat's fur color directly in your web browser using the Cataas API.

💾 Persistent Data: Automatically saves and loads all cat profiles and event history to local .csv files so you never lose your data.

🛠️ Technologies Used
Language: C++20 (utilizing <ranges>, <chrono>, and modern STL algorithms)

Testing: Google Test (gtest) for unit testing and quality assurance

External Libraries: nlohmann/json for secure API key configuration handling

🚀 Setup & Installation
Clone the repository and open the solution in Visual Studio.

Install Dependencies: Ensure the nlohmann.json NuGet package is installed for both the main application and the test project.

Configure API Key (Required for Photo feature):

Create a file named secrets.json in the root directory of the application.

Add your API key in the following format (if you don't have one, leave the string empty):

JSON
{
  "apiKey": "YOUR_API_KEY_HERE"
}
Build and Run: Compile the project using the C++20 (or Latest) language standard
