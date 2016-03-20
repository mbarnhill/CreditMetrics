template <class R> CSV<R>::CSV() { }
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
		this->push_back(R(cells));
	}
}
template <class R> const string CSV<R>::toString()
{
	string ret;
	for (size_t i = 0, n = this->size(); i < n; i++)
		ret = ret + this->at(i).toString() + "\n";
	return ret;
}
