template <class R> CSV<R>::CSV(const string& filename, size_t skipLines)
{
	ifstream file(filename);
	if (!file.good())
	{
		throw runtime_error("File does not exist");
	}
	while (!file.eof())
	{
		string strline;
		getline(file, strline);
		if (skipLines > 0)
		{
			skipLines--;
			continue;
		}
		if (strline.empty())
		{
			continue;
		}
		stringstream streamline(strline);
		vector<string> cells;
		while (!streamline.eof())
		{
			string cell;
			getline(streamline, cell, ',');
			if (cell != "")
			{
				cells.push_back(cell);
			}
		}
		if (cells.empty())
		{
			continue;
		}
		push_back(R(cells));
	}
}