using System;
using System.IO;
using System.IO.Ports;
using System.Management;
using System.Collections.Generic;
using System.Text.Json;

namespace SerialPortExample
{
    class Program
    {
        public static void Main()
        {
            string chiniseCOMDevice = FindPortByDescription("USB-SERIAL CH340");

            if (!string.IsNullOrEmpty(chiniseCOMDevice))
            {
                // Update this path to the actual location of your ConfigFile.json
                string jsonFilePath = @"../config/ConfigFile.json"; // Full path or relative path

                UpdateConfigFile(chiniseCOMDevice, jsonFilePath);
            }
            else
            {
                Console.WriteLine("COM port not found.");
            }
        }

        /**
         * @brief Retrieves all available PnP entities on the system.
         * 
         * @return Array of ManagementObject representing the available ports.
         */
        static ManagementObject[] FindPorts()
        {
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity");
                List<ManagementObject> objects = new List<ManagementObject>();

                foreach (ManagementObject obj in searcher.Get())
                {
                    objects.Add(obj);
                }
                return objects.ToArray();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return new ManagementObject[] { };
            }
        }

        /**
        * @brief Finds the COM port based on the device description.
        * 
        * @param description The device description to search for.
        * @return COM port name as a string if found; otherwise, an empty string.
        */
        static string FindPortByDescription(string description)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Description"].ToString().ToLower().Equals(description.ToLower()))
                    {
                        string comName = ParseCOMName(obj);
                        if (comName != null)
                        {
                            return comName;
                        }
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }
            return string.Empty;
        }

        /**
         * @brief Parses the COM port name from the ManagementObject.
         * 
         * @param obj The ManagementObject containing the port information.
         * @return The parsed COM port name as a string or null if not found.
         */
        static string ParseCOMName(ManagementObject obj)
        {
            string name = obj["Name"].ToString();
            int startIndex = name.LastIndexOf("(");
            int endIndex = name.LastIndexOf(")");

            if (startIndex != -1 && endIndex != -1)
            {
                name = name.Substring(startIndex + 1, endIndex - startIndex - 1);
                return name;
            }
            return null;
        }

        /**
         * @brief Updates the COM port information in the configuration file.
         * 
         * @param comPort The COM port to be updated in the config file.
         * @param fileName The path to the configuration file (JSON format).
         */
        static void UpdateConfigFile(string comPort, string fileName)
        {
            try
            {
                // Read the existing config file
                string jsonString = File.ReadAllText(fileName);
                // Deserialize the JSON file to an object
                var config = JsonSerializer.Deserialize<ArduinoConfig>(jsonString);

                // Only update the ArduinoCOMPort, leave ArduinoBaudRate unchanged
                if (config?.ArduinoSettings != null)
                {
                    config.ArduinoSettings.COMPort = comPort;
                }

                // Serialize the updated object back to JSON with indents
                jsonString = JsonSerializer.Serialize(config, new JsonSerializerOptions { WriteIndented = true });

                // Write the updated JSON back to the file
                File.WriteAllText(fileName, jsonString);

                Console.WriteLine($"[INFO] COM port '{comPort}' has been written to the config file {fileName}");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while updating the config file: " + e.Message);
            }
        }
    }

    /**
    * @brief Represents the structure of the Arduino configuration file.
    */
    public class ArduinoConfig
    {
        public ArduinoSettings ArduinoSettings { get; set; }
    }

    /**
    * @brief Represents the Arduino settings within the configuration file.
    */
    public class ArduinoSettings
    {
        public string COMPort { get; set; }
    }
}
