
function help(topic)
{
	function HelpTopic(topic, shortTopic, helpText)
	{
		this.topic = topic;
		this.shortTopic = shortTopic;
		this.helpText = helpText;

		this.toHelp = function (){
			return ">" + this.topic + "<\n\t"  + this.shortTopic +
				"\n\n" + this.helpText;
		}

		return this;
	}

	HelpTopic.prototype.toString = function()
	{
		spaces = "";
		for(var i = this.topic.length; i < 15; i++, spaces += " ");

		return " * " + this.topic + spaces +" - "+ this.shortTopic
	}

	function HelpObject()
	{
		this.genericHelp = "Display help about selected topic. \nAvailable topics:"
	}

	HelpObject.prototype.topics = help.topics;

	HelpObject.prototype.registerTopic = function (topic, shortTopic, helpText)
	{
		this.topics.push(new HelpTopic(topic, shortTopic, helpText));
		return null;
	}

	HelpObject.prototype.toString = function()
	{
		items = ""

		for (var i = 0; i < this.topics.length; i++) {
			items += this.topics[i]+ "\n";
		};

		return this.genericHelp + "\n" + items;
	}

	function findHelp(topic)
	{
		if(topic)
		{
			for (var i = 0; i < help.topics.length; i++) {
				if(help.topics[i].topic == topic){
					return help.topics[i].toHelp();

				}
			};

			if(typeof topic.help == "function")
				return topic.help();

			if(repleisiukas && repleisiukas.hasOwnProperty(topic) && repleisiukas[topic] && typeof repleisiukas[topic].help == "function"){
				return repleisiukas[topic].help();
			}
		}

		return "Could not find help on topic '" + topic + "'";
	}

	if(topic){
		return findHelp(topic);
	}
	else
	{
		return new HelpObject();
	}
}
help.topics = [];

help().registerTopic("help", "Help system.",
"Displays and manages help topics.\n\n\t* Displaying\n\
use function help() to get available help topics. \n\
use help(\"topic\") to get help about specific help topic (e.g. help(\"help\"))\n\n\
\t* Managing\n\
Custom help topics can be registered using function help().registerTopic('Topic', 'Short Description', 'Full Help Text')\n\
");


help().registerTopic("dialogizuoti", "Sugeneruojama forma, kurioje patogiau įvesti kintamuosius.",
"Duomenų įvedimo laukelyje įrašykite dialogizavimo formatą ir iš kontekstinio meniu pasirinkite punktą „Dialogizuoti“.\
\nPavyzdys, skripte galėsite naudoti kintamuosiuos variable ir variable1:\n\
text variable=default;;\nnumber variable1=123;;\n\n\
Palaikomi laukelių tipai:\n\
	* number\n\
	* text\n\
Automatinio užkrovimo žymos, kurios gali būti panaudotos failuose automatiniam dialogizavimui įkeliant skriptą:\n\
\"--<autoload>--\" ir \"--</autoload>--\"\n\
");
