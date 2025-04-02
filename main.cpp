//
// Created by MattFor on 31.03.2025.
//

#include <thread>
#include <chrono>
#include <ranges>
#include <fstream>
#include <imgui.h>
#include <functional>
#include <unordered_set>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_hints.h>
#include <imgui_impl_glfw.h>
#include <SDL3/SDL_oldnames.h>
#include <imgui_impl_opengl3.h>

#include "logic.cpp"

int main()
{
    // Obsolete because I don't know how to use this yet
    SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "directsound");
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_Log("Failed to initialize SDL audio: %s", SDL_GetError());
        return 1;
    }

    int comparisonCount = 0;
    int arrayAccessCount = 0;

    bool repeating = false;
    bool randomized = false;
    bool after_settings = false;

    srand(static_cast<unsigned int>(time(nullptr)));

    auto mode = MODE::SORTING;

    Settings settings{};
    std::vector<Value> values;
    float maximum_value = 0.0;

    // Read the settings and get the initial sorting function
    auto sortingStep = read_settings(settings, values, maximum_value, repeating, arrayAccessCount, comparisonCount);

    if (settings.performanceMeasurement == "y" || settings.performanceMeasurement == "Y")
    {
        std::vector<int> sizes;
        const bool multiTesting = settings.multipleTests == "y" || settings.multipleTests == "Y";
        if (multiTesting)
        {
            sizes = {10, 50, 100, 300, 750, 1000, 3000, 4500, 6000, 7000, 8000, 9000, 10000}; //, 15000, 20000, 30000, 40000, 200000, 500000, 1000000};
        }
        else
        {
            sizes.push_back(settings.numValues);
        }

        // Define the conversion map for test types
        std::unordered_map<PERFORMANCE_MODE, std::string> conversion =
        {
            { PERFORMANCE_MODE::MIXED,     "Mixed"          },
            { PERFORMANCE_MODE::POSITIVES, "All Positive"   },
            { PERFORMANCE_MODE::NEGATIVES, "All Negative"   },
            { PERFORMANCE_MODE::SORTED,    "Already Sorted" },
            { PERFORMANCE_MODE::REVERSED,  "Reverse Sorted" }
        };

        // Store results as a tuple (TestType, ArraySize, Duration_ms)
        std::unordered_map<ALGORITHM, std::vector<std::tuple<std::string, int, double>>> results;

        // Full algorithm names for displaying in the progress messages
        std::unordered_map<ALGORITHM, std::string> algorithmNamesOnIncluded =
        {
            { ALGORITHM::BUBBLE,    "Bubble Sort (O(n^2))"                      },
            { ALGORITHM::INSERTION, "Insertion Sort (O(n^2) worst | O(n) best)" },
            { ALGORITHM::SELECTION, "Selection Sort (O(n^2))"                   },
            { ALGORITHM::QUICKSORT, "Quicksort (O(n log n) avg | O(n^2) worst)" },
            { ALGORITHM::SHELL,     "Shell Sort (O(n log^2 n))"                 },
            { ALGORITHM::HEAP,      "Heap Sort (O(n log n))"                    },
            { ALGORITHM::RADIX,     "Radix LSD Bucket Sort (O(nk))"             },
            { ALGORITHM::COCKTAIL,  "Cocktail Sort (O(n^2))"                    },
            { ALGORITHM::BOGO,      "Bogo Sort (O(n!))"                         },
            { ALGORITHM::TIMSORT,   "Timsort (O(n) best | O(n log n) avg)"      },
            { ALGORITHM::GNOMESORT, "Gnome Sort (O(n^2) worst | O(n) best)"     },
            { ALGORITHM::SPREADSORT,"Spreadsort (Simplified) (near-linear)"     }
        };

        // A simpler set of names for CSV output
        std::unordered_map<ALGORITHM, std::string> algorithmNames =
        {
            { ALGORITHM::BUBBLE,    "Bubble Sort"           },
            { ALGORITHM::INSERTION, "Insertion Sort"        },
            { ALGORITHM::SELECTION, "Selection Sort"        },
            { ALGORITHM::QUICKSORT, "Quicksort"             },
            { ALGORITHM::SHELL,     "Shell Sort"            },
            { ALGORITHM::HEAP,      "Heap Sort"             },
            { ALGORITHM::RADIX,     "Radix LSD Bucket Sort" },
            { ALGORITHM::COCKTAIL,  "Cocktail Sort"         },
            { ALGORITHM::BOGO,      "Bogo Sort"             },
            { ALGORITHM::TIMSORT,   "Timsort"               },
            { ALGORITHM::GNOMESORT, "Gnome Sort"            },
            { ALGORITHM::SPREADSORT,"Spreadsort"            }
        };

        // Exclude certain algorithms
        std::unordered_set excluded =
        {
            ALGORITHM::BOGO
        };

        // Determine the total number of tests
        // Total tests = (# of algorithms not excluded) * (# of test types [5]) * (# of sizes)
        int totalTests = 0;
        int testTypesCount = 5;
        int sizesCount = static_cast<int>(sizes.size());
        for (const auto & algo : algoConfig | std::views::keys)
        {
            if (!excluded.contains(algo))
            {
                totalTests += testTypesCount * sizesCount;
            }
        }

        int currentTest = 0;

        for (const auto & [algo, config] : algoConfig)
        {
            if (excluded.contains(algo))
            {
                continue;
            }

            SortingAlgorithm func = config.func;
            std::vector<std::tuple<std::string, int, double>> algoResults;

            // Loop over each array size
            for (int size : sizes)
            {
                // Loop over each test type
                for (int i = 0; i < testTypesCount; i++)
                {
                    const auto perf_mode = static_cast<PERFORMANCE_MODE>(i);
                    currentTest++;

                    int tabCount;
                    std::string algName = algorithmNamesOnIncluded[algo];

                    if (algName.size() < 25)
                    {
                        tabCount = 3;
                    }
                    else if (algName.size() < 35)
                    {
                        tabCount = 2;
                    }
                    else
                    {
                        tabCount = 1;

                        // Too large
                        if (algName == "Insertion Sort (O(n^2) worst | O(n) best)")
                        {
                            tabCount = 0;
                        }
                    }

                    std::string tabStr(tabCount, '\t');
                    std::cout << "Running test " << currentTest << " of " << totalTests
                              << ": " << algName << tabStr
                              << "\t| " << conversion[perf_mode]
                              << (conversion[perf_mode] == "All Negative" ||
                                  conversion[perf_mode] == "All Positive" ? "\t\t" :
                                  conversion[perf_mode] == "Mixed" ? "\t\t\t" : "\t")
                              << "| Array Size = " << size << '\n';

                    // Generate test values with the given array size
                    std::vector<Value> testValues = generate_values(perf_mode, size, settings.windowHeight, VALUE_TYPE::INTEGER);

                    auto sortFunction = make_sorting_function(func, arrayAccessCount, comparisonCount, -10000);
                    auto startTime = std::chrono::high_resolution_clock::now();
                    while (!sortFunction(testValues, arrayAccessCount, comparisonCount, -10000))
                    {
                        // Loop until sorting is complete
                    }

                    auto endTime = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> duration = endTime - startTime;

                    algoResults.emplace_back(conversion[perf_mode], size, duration.count());
                    std::cout << "> " << conversion[perf_mode] << ", Array Size = " << size
                              << ": " << duration.count() << " ms" << '\n';
                }
            }

            results[algo] = algoResults;
        }

        std::cout << "\nPerformance Measurement Results:\n";
        for (const auto & [algo, algoResults] : results)
        {
            std::cout << algorithmNamesOnIncluded[algo] << ":\n";
            for (const auto & tup : algoResults)
            {
                std::string testType;
                int arraySize;
                double timeMs;
                std::tie(testType, arraySize, timeMs) = tup;
                std::cout << "> " << testType << ", Array Size = " << arraySize << ": " << timeMs << " ms" << '\n';
            }
        }

        // Save results to file in a csv-able format
        std::string filename = multiTesting ? "../results/performance_results_multi.txt" : "../results/performance_results.txt";
        if (std::ofstream outfile(filename); outfile.is_open())
        {
            outfile << "Algorithm,TestType,ArraySize,Duration_ms\n";
            for (const auto & [algo, algoResults] : results)
            {
                for (const auto & tup : algoResults)
                {
                    std::string testType;
                    int arraySize;
                    double timeMs;
                    std::tie(testType, arraySize, timeMs) = tup;
                    outfile << algorithmNames[algo] << "," << testType << "," << arraySize << "," << timeMs << "\n";
                }
            }

            outfile.close();
            std::cout << "\nResults saved to " << filename << "\n";
        }
        else
        {
            std::cerr << "Error opening file for writing results.\n";
        }

        std::cout << "\nWould you like to draw the graphs? (y/n):\n";
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "y" || answer == "Y")
        {
            if (int ret = std::system("python ../plot_results.py"); ret != 0)
            {
                std::cerr << "Error running the Python plotting script.\n";
            }
        }

        std::cout << "Press Enter to close performance monitor...";
        std::cin.get();

        return 0;
    }

    if (!glfwInit())
    {
        return -1;
    }

    /**
     * Initialize the window and make sure that it stays on top and is focused once created
     */
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(settings.windowWidth, settings.windowHeight, "Sorting Visualizer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowUserPointer(window, &settings);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, settings.windowWidth, 0, settings.windowHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glfwSwapInterval(0);

    // Initialize ImGui here for real-time performance statistics
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    update_bar_positions(values, settings.windowWidth);

    int step = 0;

    bool sorted = false;
    bool pWasPressed = false;
    bool zWasPressed = false;
    bool enterWasPressed = false;
    bool settingsTriggered = false;

    auto idleStart = std::chrono::steady_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (
            mode != MODE::SETTINGS &&
            (
                glfwGetKey(window, GLFW_KEY_ENTER)  == GLFW_PRESS    ||
                glfwGetKey(window, GLFW_KEY_P)      == GLFW_PRESS    ||
                glfwGetKey(window, GLFW_KEY_Z)      == GLFW_PRESS    ||
                glfwGetKey(window, GLFW_KEY_1)      == GLFW_PRESS
            )
        )
        {
            idleStart = std::chrono::steady_clock::now();
            settingsTriggered = false; // reset a trigger flag if any key is pressed
        }

        if (mode == MODE::COMPLETED)
        {
            step++;
            repeating = true;
        }

        if (step >= static_cast<int>(values.size()))
        {
            step = 0;
            mode = MODE::IDLE;
            idleStart = std::chrono::steady_clock::now();
        }

        update_bar_positions(values, settings.windowWidth);

        // In IDLE mode, check for new key or idle timeout
        if (mode == MODE::IDLE)
        {
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            {
                mode = MODE::SETTINGS;
            }
            else
            {
                auto now = std::chrono::steady_clock::now();

                if (auto idleDuration = std::chrono::duration_cast<std::chrono::seconds>(now - idleStart); idleDuration.count() >= 10 && !settingsTriggered)
                {
                    comparisonCount = 0;
                    arrayAccessCount = 0;

                    std::cout << "\nIdle for 10 seconds. Showing settings menu...\n";
                    mode = MODE::SETTINGS;
                    settingsTriggered = true;
                }
            }
        }

        // When in SETTINGS mode, block to get new parameters
        if (mode == MODE::SETTINGS)
        {
            sortingStep = read_settings(settings, values, maximum_value, repeating, arrayAccessCount, comparisonCount);

            // Reset a sorted flag because we are starting a new sort.
            sorted = false;

            // Reset idle timer.
            idleStart = std::chrono::steady_clock::now();
            after_settings = true;

            if (!randomized)
            {
                std::cout << "Randomizing...\n";
                mode = MODE::RANDOMIZING;
            }
        }

        float currentDelay = 0;

        const int sound = settings.sound + static_cast<int>(settings.delaySeconds) + 1;

        // Handle how we advance through the graph
        if (settings.automatic || mode == MODE::RANDOMIZING)
        {
            if (settings.delaySeconds > 0 && mode != MODE::COMPLETED && mode != MODE::RANDOMIZING)
            {
                currentDelay = settings.delaySeconds * 1000;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(currentDelay)));
            }

            if (mode == MODE::SORTING || (after_settings && mode != MODE::RANDOMIZING))
            {
                randomized = false;
                execute_logic(values, sortingStep, mode, sorted, arrayAccessCount, comparisonCount, sound);

                /**
                 * If we are right after changing settings, only after executing logic do we need to change to sorting mode
                 * has we done that immediately, the program would seize up
                 **/
                if (after_settings)
                {
                    mode = MODE::SORTING;
                    after_settings = false;
                }
            }
            else if (mode == MODE::RANDOMIZING)
            {
                currentDelay = /* settings.delaySeconds > 0.1 ? settings.delaySeconds * 1000 : */ static_cast<float>(values.size()) / 100;

                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(currentDelay)));

                if (randomize_step(values, arrayAccessCount, comparisonCount, sound))
                {
                    /**
                     * After randomizing, go into idle mode and initiate the settings interval
                     **/
                    mode = MODE::IDLE;
                    randomized = true;

                    if (!after_settings)
                    {
                        idleStart = std::chrono::steady_clock::now();
                    }
                }
            }
        }
        else // Manual stepping
        {
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterWasPressed)
            {
                if (mode == MODE::SORTING)
                {
                    execute_logic(values, sortingStep, mode, sorted, arrayAccessCount, comparisonCount, sound);
                }
                else if (mode == MODE::RANDOMIZING)
                {
                    currentDelay = settings.delaySeconds > 0.0 ? settings.delaySeconds * 1000 : static_cast<float>(values.size()) / 100;
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(currentDelay)));

                    if (randomize_step(values, arrayAccessCount, comparisonCount, sound))
                    {
                        mode = MODE::IDLE;
                        idleStart = std::chrono::steady_clock::now();
                    }
                }
                enterWasPressed = true;
            }

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
            {
                enterWasPressed = false;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pWasPressed)
        {
            print_values(values);
            pWasPressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
        {
            pWasPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !zWasPressed)
        {
            zWasPressed = true;
            comparisonCount = 0;
            arrayAccessCount = 0;
            mode = MODE::RANDOMIZING;
            std::cout << "Randomizing...\n";
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
        {
            zWasPressed = false;
        }

        if (mode == MODE::COMPLETED)
        {
            currentDelay = values.size() > 100 ? 0 : 10;
            std::this_thread::sleep_for(std::chrono::milliseconds(values.size() > 100 ? 0 : 10));
        }

        // Draw the bars
        draw_values(values, static_cast<float>(settings.windowWidth) / static_cast<float>(values.size()), static_cast<float>(settings.windowHeight), maximum_value, mode, step, sound);

        // Draw the ImGuI statistics
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(250, 100)); // Set a wider window size
        ImGui::Begin("Statistics", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
        ImGui::SetWindowPos(ImVec2(10, 10));
        ImGui::Text("Delay: %0.2fms", currentDelay);
        ImGui::Text("Comparisons: %d", comparisonCount);
        ImGui::Text("Array accesses: %d", arrayAccessCount);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free all the memory
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
